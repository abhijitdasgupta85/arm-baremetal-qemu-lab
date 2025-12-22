#ifndef UART_PL011_H
#define UART_PL011_H

#include <stdint.h>

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_puthex(uint32_t value);

#endif
