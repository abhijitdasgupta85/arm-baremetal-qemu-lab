#ifndef UART_PL011_H
#define UART_PL011_H

#include <types.h>

/* =========================================================
 * PL011 UART definitions for QEMU virt
 * ========================================================= */

/* Base address */
#define UART0_BASE   0x09000000UL

/* Register offsets */
#define UART_DR      0x00
#define UART_FR      0x18
#define UART_IBRD    0x24
#define UART_FBRD    0x28
#define UART_LCRH    0x2C
#define UART_CR      0x30

/* Flag bits */
#define UART_FR_TXFF (1 << 5)  /* Transmit FIFO full */
#define UART_FR_RXFE (1 << 4)  /* Receive FIFO empty */

/* =========================================================
 * Public API
 * ========================================================= */

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);

/* RX handling */
void uart_rx_poll(void);
int32_t uart_read_nb(char *c);

#endif /* UART_PL011_H */
