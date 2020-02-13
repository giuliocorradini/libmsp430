#ifndef MSP430_H
#include <msp430.h>
#endif
#ifndef STDINT_H
#include <stdint.h>
#endif


//Configuration
void timer_init();
void timer_set_callback(void* callback_function);


//Flow control
void timer_start();
void timer_stop();
void timer_reset();


//Query
uint64_t millis();
