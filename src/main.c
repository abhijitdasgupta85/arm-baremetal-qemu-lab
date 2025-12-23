#include <string.h>
#include "uart_pl011.h"

void main(void)
{
    uart_init();

    uart_puts("\nUART Echo Test (QEMU Cortex-A)\n");
    uart_puts("Type characters to see them echoed back:\n");

    while (1)
    {
        char buffer[64];
        char c = uart_getc();   /* blocking read */
        
        strcpy(buffer, "Received Data: ");
        strcat(buffer, c);
        uart_puts(buffer);

        uart_putc(c);           /* echo */
    }
}
