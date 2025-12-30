#include "uart_pl011.h"
#include "ringbuf.h"

/* =========================================================
 * Private RX buffer
 * ========================================================= */
static ringbuf_t uart_rx_buf;

/* =========================================================
 * MMIO helpers (private to this driver)
 * ========================================================= */
static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}

static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

/* =========================================================
 * UART initialization
 * ========================================================= */
void uart_init(void)
{
    ringbuf_init(&uart_rx_buf);

    mmio_write(UART0_BASE + UART_CR, 0x0);

    mmio_write(UART0_BASE + UART_IBRD, 1);
    mmio_write(UART0_BASE + UART_FBRD, 40);
    mmio_write(UART0_BASE + UART_LCRH, (3 << 5)); /* 8N1 */

    /* Enable UART, TX, RX */
    mmio_write(UART0_BASE + UART_CR, (1 << 9) | (1 << 8) | 1);
}

/* =========================================================
 * TX
 * ========================================================= */
void uart_putc(char c)
{
    while (mmio_read(UART0_BASE + UART_FR) & UART_FR_TXFF)
        ;
    mmio_write(UART0_BASE + UART_DR, c);
}

void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

/* =========================================================
 * RX
 * ========================================================= */
static inline int uart_rx_ready(void)
{
    return !(mmio_read(UART0_BASE + UART_FR) & UART_FR_RXFE);
}

void uart_rx_poll(void)
{
    while (uart_rx_ready()) {
        ringbuf_put(&uart_rx_buf,
                    mmio_read(UART0_BASE + UART_DR) & 0xFF);
    }
}

int32_t uart_read_nb(char *c)
{
    return ringbuf_get(&uart_rx_buf, c);
}
