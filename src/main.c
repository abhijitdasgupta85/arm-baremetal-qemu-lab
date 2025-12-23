#include "uart_pl011.h"

static void my_strcpy(char *dst, const char *src)
{
    while ((*dst++ = *src++));
}

static void my_strcat(char *dst, const char *src)
{
    while (*dst) dst++;
    while ((*dst++ = *src++));
}

int main(void)
{
    uart_init();

    uart_puts("\nUART Echo Test (QEMU Cortex-A)\n");
    uart_puts("Type characters to see them echoed back:\n");

    while (1)
    {
        char ch_str[2];
        char c;
        char buffer[64];

        c = uart_getc();   /* blocking read */
        ch_str[0] = c;
        ch_str[1] = '\0';

        my_strcpy(buffer, "Received Data: ");
        my_strcat(buffer, ch_str);
        
        uart_puts(buffer);
        uart_puts("\r\n");

        uart_putc(c);     /* echo */
    }
}
