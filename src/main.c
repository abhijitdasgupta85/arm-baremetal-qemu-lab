#include "uart_pl011.h"

void main(void)
{
    uart_init();
    uart_puts("Boot OK - Cortex-A bare metal\n");

    //while (1);
}
