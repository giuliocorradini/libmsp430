#include <msp430.h>
#include "uart.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    UART uart;
    uart_global_config(bps9600);
    uart_init(&uart);

    uint8_t buf;

    while(true) {
        uart.read((char*)&buf, 1);
        uart.write(&buf, 1);
    }

    return 0;
}