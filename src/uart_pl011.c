#include "uart_pl011.h"
#include <stdint.h>

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

static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}

static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

void uart_init(void)
{
    mmio_write(UART0_BASE + UART_CR, 0x0);

    mmio_write(UART0_BASE + UART_IBRD, 1);
    mmio_write(UART0_BASE + UART_FBRD, 40);

    mmio_write(UART0_BASE + UART_LCRH, (3 << 5)); /* 8N1 */

    mmio_write(UART0_BASE + UART_CR, (1 << 9) | (1 << 8) | 1);
}

void uart_putc(char c)
{
    while (mmio_read(UART0_BASE + UART_FR) & UART_FR_TXFF)
        ;
    mmio_write(UART0_BASE + UART_DR, c);
}

char uart_getc(void)
{
    while (mmio_read(UART0_BASE + UART_FR) & UART_FR_RXFE)
        ;
    return (char)(mmio_read(UART0_BASE + UART_DR) & 0xFF);
}

void uart_puts(const char *s)
{
    while (*s)
    {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}
