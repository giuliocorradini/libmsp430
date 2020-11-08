#include "uart.h"
#include <string.h>

#define UART_RX_BUFFER_SIZE 128
static circular_buffer_t __buffer;

const uint16_t prescaler_val[n_speeds] = {
                                          109,  //9600bps
                                          54,   //19200bps
                                          27,   //38400bps
                                          18,   //57600bps
                                          9     //115200bps
};

const uint8_t modulator_val[n_speeds] = {
                                         0,     //9600bps
                                         UCBRS_5,     //19200bps
                                         UCBRS_2,     //38400bps
                                         UCBRS_1,     //57600bps
                                         UCBRS_1      //115200bps
};

void uart_config(enum UART_speed_t baudrate) {

    uint16_t prescaler = prescaler_val[baudrate];
    uint8_t modulator = modulator_val[baudrate];

    UCA1CTL1 |= UCSWRST;    //Reset controller, enter config mode

    UCA1CTL1 |= UCSSEL_2;

    UCA1BRW = prescaler;
    UCA1MCTL |= modulator;

    P4SEL |= (BIT4 | BIT5);

    UCA1MCTL = UCBRS_2 + UCBRF_0;

    UCA1CTL1 &= ~UCSWRST;   //Exit config mode


    UCA1IE |= UCRXIE;
    __enable_interrupt();

    cbInit(&__buffer, UART_RX_BUFFER_SIZE);

}

void uart_putchar(char ch) {
    while(!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = ch;
}

char uart_getchar() {
    char c;
    while(!cbRead(&__buffer, &c));;

    return c;
}

char uart_async_getchar(int *err) {
    char c;
    if(cbRead(&__buffer, &c)) {
        return c;
    } else {
        *err = 1;
        return 0x00;
    }
}

void uart_write(const char* src, int n) {

    for(int i=0; i<n; i++) {
        uart_putchar(src[i]);
    }

}

void uart_writeline(const char* str) {
    uart_write(str, strlen(str));
}

void uart_read(char* dst, int n) {

    for(int i=0; i<n; i++) {
        dst[i] = uart_getchar();
    }

}

int uart_async_read(char* dst, int n) {
    int read_bytes = 0;
    while(!cbIsEmpty(&__buffer) && read_bytes < n) {
        cbRead(&__buffer, dst+read_bytes);
        read_bytes++;
    }
}


#pragma vector = USCI_A1_VECTOR
__interrupt void uart_read_isr(void) {

    cbWrite(&__buffer, UCA1RXBUF);

}
