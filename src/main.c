#include "uart_pl011.h"

void main(void)
{
    uart_init();

    uart_puts("\nUART Echo Test (QEMU Cortex-A)\n");
    uart_puts("Type characters to see them echoed back:\n");

    while (1)
    {
        char c = uart_getc();   /* blocking read */
        uart_putc(c);           /* echo */
    }
}
