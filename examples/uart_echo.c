#include <msp430.h>
#include "uart.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    uart_config(bps9600);
    uint8_t buf;

    while(true) {
        uart_read(&buf, 1);
        uart_write(&buf, 1);
    }

    return 0;
}