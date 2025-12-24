#include "uart_pl011.h"
#include "mystring.h"
#include "ringbuf.h"

#define RX_MSG_MAX_LEN 64

int32_t main(void)
{
    char c;
    char rx_msg[RX_MSG_MAX_LEN];
    uint32_t idx = 0;

    uart_init();
    uart_puts("[M2] Core Services Ready\n");
    uart_puts("Waiting for UART messages...\n");

    while (1) {
    /* Always drain RX FIFO first */
    uart_rx_poll();

    while (uart_read_nb(&c)) {

        if (c == '\n' || c == '\r') {
            rx_msg[idx] = '\0';

            uart_puts("[RX] ");
            uart_puts(rx_msg);
            uart_puts("\n");

            idx = 0;
        } else {
            if (idx < RX_MSG_MAX_LEN - 1) {
                rx_msg[idx++] = c;
            }
        }
    }
}

}
