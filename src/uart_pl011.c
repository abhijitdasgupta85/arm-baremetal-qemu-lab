#include "uart_pl011.h"
#include <stdint.h>

/*
 * QEMU virt machine
 * PL011 UART base address
 */
#define UART0_BASE   0x09000000UL

/* PL011 register offsets */
#define UART_DR      0x00  /* Data Register */
#define UART_FR      0x18  /* Flag Register */
#define UART_IBRD    0x24  /* Integer Baud Rate */
#define UART_FBRD    0x28  /* Fractional Baud Rate */
#define UART_LCRH    0x2C  /* Line Control */
#define UART_CR      0x30  /* Control Register */

/* Flag Register bits */
#define UART_FR_TXFF (1 << 5)  /* Transmit FIFO full */

static inline void mmio_write(uint32_t reg, uint32_t data)
{
    *(volatile uint32_t *)reg = data;
}

static inline uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t *)reg;
}

/*
 * Initialize UART
 * QEMU virt already sets clock, so minimal init is sufficient
 */
void uart_init(void)
{
    /* Disable UART */
    mmio_write(UART0_BASE + UART_CR, 0x0);

    /* 115200 baud (works reliably in QEMU) */
    mmio_write(UART0_BASE + UART_IBRD, 1);
    mmio_write(UART0_BASE + UART_FBRD, 40);

    /* 8-bit, no parity, 1 stop bit, FIFO enabled */
    mmio_write(UART0_BASE + UART_LCRH, (3 << 5));

    /* Enable UART, TX, RX */
    mmio_write(UART0_BASE + UART_CR, (1 << 9) | (1 << 8) | 1);
}

/*
 * Transmit a single character
 */
void uart_putc(char c)
{
    /* Wait until TX FIFO has space */
    while (mmio_read(UART0_BASE + UART_FR) & UART_FR_TXFF)
        ;

    mmio_write(UART0_BASE + UART_DR, (uint32_t)c);
}

/*
 * Transmit a string
 */
void uart_puts(const char *s)
{
    while (*s)
    {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

/*
 * Optional: print hex (useful for debugging scheduler)
 */
void uart_puthex(uint32_t value)
{
    const char hex[] = "0123456789ABCDEF";

    uart_puts("0x");
    for (int i = 28; i >= 0; i -= 4)
        uart_putc(hex[(value >> i) & 0xF]);
}
