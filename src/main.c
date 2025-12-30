#include "uart_pl011.h"


#define RX_BUF_LEN 128

int32_t main(void)
{
    char c;
    char rx_buf[RX_BUF_LEN];
    int idx = 0;

    uart_init();

    uart_puts("[M2] Core Services Ready\n");
    uart_puts("Waiting for UART messages...\n");

    while (1) {
        /* Drain HW FIFO into software ring buffer */
        uart_rx_poll();

        /* Drain software ring buffer */
        while (uart_read_nb(&c)) {

            if (c == '\n' || c == '\r') {
                rx_buf[idx] = '\0';

                uart_puts("[RX] ");
                uart_puts(rx_buf);
                uart_puts("\n");

                idx = 0;
            } else {
                if (idx < RX_BUF_LEN - 1) {
                    rx_buf[idx++] = c;
                }
            }
        }
    }
}
