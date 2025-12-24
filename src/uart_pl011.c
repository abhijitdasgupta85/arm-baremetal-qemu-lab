#include "uart_pl011.h"
#include "ringbuf.h"

#define UART0_BASE   0x09000000UL

#define UART_DR      0x00
#define UART_FR      0x18
#define UART_IBRD    0x24
#define UART_FBRD    0x28
#define UART_LCRH    0x2C
#define UART_CR      0x30

/* Flag bits */
#define UART_FR_TXFF (1 << 5)  /* Transmit FIFO full */
#define UART_FR_RXFE (1 << 4)  /* Receive FIFO empty */

static ringbuf_t uart_rx_buf;

/*************************************************
 * @brief Read data from the hardware buffer register
 *
 * @details
 *
 * @return None.
 *************************************************/
static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}

/*************************************************
 * @brief Write data to the hardware buffer register
 *
 * @details
 *
 * @return None.
 *************************************************/
static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

/*************************************************
 * @brief Initializes UART hardware registers.
 *
 * @details
 * Configures baud rate, data bits, parity, and stop bits.
 * This function must be called once during system startup.
 *
 * @return None.
 *************************************************/
void uart_init(void)
{
    mmio_write(UART0_BASE + UART_CR, 0x0);

    mmio_write(UART0_BASE + UART_IBRD, 1);
    mmio_write(UART0_BASE + UART_FBRD, 40);

    mmio_write(UART0_BASE + UART_LCRH, (3 << 5)); /* 8N1 */

    mmio_write(UART0_BASE + UART_CR, (1 << 9) | (1 << 8) | 1);
}

/*************************************************
 * @brief writes character onto the output buffer.
 *
 * @details writes character onto the output buffer.
 *
 * @return None.
 *************************************************/
void uart_putc(char c)
{
    while (mmio_read(UART0_BASE + UART_FR) & UART_FR_TXFF)
        ;
    mmio_write(UART0_BASE + UART_DR, c);
}

/*************************************************
 * @brief writes string onto the output buffer.
 *
 * @details writes each character of the input string onto the output buffer.
 *
 * @return None.
 *************************************************/
void uart_puts(const char *s)
{
    while (*s)
    {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

/*************************************************
 * @brief is uart rx ready.
 *
 * @details 
 *
 * @return None.
 *************************************************/
int32_t uart_rx_ready(void)
{
    return !(UART_FR & (1 << 4)); // RXFE = 0 → data available
}

/*************************************************
 * @brief writes string onto the output buffer.
 *
 * @details 
 *
 * @return None.
 *************************************************/
char uart_getc(void)
{
    while (!uart_rx_ready());
        return UART_DR & 0xFF;
}

/*************************************************
 * @brief Receive poll function
 *
 * @details 
 *
 * @return None.
 *************************************************/
void uart_rx_poll(void)
{
    while (uart_rx_ready()) {
        ringbuf_put(&uart_rx_buf, uart_getc());
    }
}

/*************************************************
 * @brief Receive poll function
 *
 * @details 
 *
 * @return None.
 *************************************************/
int32_t uart_read_nb(char *c)
{
    return ringbuf_get(&uart_rx_buf, c);
}
