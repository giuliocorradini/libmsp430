#include <uart.h>
#include <stdbool.h>

const uint16_t prescaler_val[n_speeds] = {
                                          109,  //9600bps
                                          54,   //19200bps
                                          27,   //38400bps
                                          18,   //57600bps
                                          9     //115200bps
};

const uint8_t modulator_val[n_speeds] = {
                                         UCBRS_2,     //9600bps
                                         UCBRS_5,     //19200bps
                                         UCBRS_2,     //38400bps
                                         UCBRS_1,     //57600bps
                                         UCBRS_1      //115200bps
};

void uart_write(const uint8_t* data, int data_len) {

    int i;
    for(i=0; i<data_len; i++) {
        while(!(UCA1IFG & UCTXIFG));   //wait until buffer is empty
        UCA1TXBUF = data[i];
    }

}

typedef struct __ring_buffer {
    uint8_t *pool;
    unsigned int size;
    unsigned int head;
    unsigned int tail;
    unsigned int elements;
} buffer_t;

static uint8_t buffer_rx_pool[256];
static buffer_t rx_buffer = {.pool = buffer_rx_pool, .size = 256, .head=0, .tail=0, .elements=0};

void uart_read(char* buffer, int buffer_size) {

    unsigned int read_bytes = 0;

    while(read_bytes != buffer_size) {
        while(rx_buffer.elements == 0);;
        buffer[read_bytes++] = rx_buffer.pool[rx_buffer.head++];
        rx_buffer.head %= rx_buffer.size;
        rx_buffer.elements--;
    }

}

void uart_global_config(enum UART_speed_t baudrate) {

    uint16_t prescaler = prescaler_val[baudrate];
    uint8_t modulator = modulator_val[baudrate];

    UCA1CTL1 |= UCSWRST;    //Reset controller, enter config mode

    UCA1CTL1 |= UCSSEL_2;

    UCA1BRW = prescaler;
    UCA1MCTL |= modulator;

    P4SEL |= (BIT4 | BIT5);


    UCA1CTL1 &= ~UCSWRST;   //Exit config mode


    UCA1IE |= UCRXIE;
    __enable_interrupt();

}

void uart_init(UART *uc) {
    uc->write = uart_write;
    uc->read = uart_read;
}


#pragma vector = USCI_A1_VECTOR
__interrupt void uart_read_isr(void) {

    if(rx_buffer.head != rx_buffer.tail || rx_buffer.elements == 0) {
        rx_buffer.pool[rx_buffer.tail++] = UCA1RXBUF;
        rx_buffer.tail %= rx_buffer.size;
        rx_buffer.elements++;
    } else { //Full buffer, discard incoming bytes
        UCA1IFG &= ~UCRXIFG;
    }

}
