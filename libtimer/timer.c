/*
 * timer.c
 *
 *  Created on: 21 gen 2020
 *      Author: Giulio Corradini
 */

#include <msp430.h>
#include <stdint.h>
#include <timer.h>

static uint64_t millisecond_counter = 0;

void default_callback_function() {
    //Do absolutely nothing
}

void (callback*)() = &default_callback_function;

void timer_init() {
    //Clear timer
    TA0CTL |= TACLR;

    //Configuring registers for Timer A0
    TA0CTL |= TASSEL_1; //ACLK

    //Enabling flags
    TA0CTL |= TAIE;
    TA0CTL &= ~TAIFG;

    TA0CCTL0 |= CCIE;

    //Count up to 32 (1ms)
    TA0CCR0 = 0x20;

    __enable_interrupt();
}

void timer_set_callback(void (user_callback_function*)()) {
    callback = &user_callback_function;
}



void timer_start() {
    TA0CTL &= ~TAIFG;
    TA0CTL |= MC_1;
}

void timer_stop() {
    TA0CTL &= ~MC_0;
}

void timer_reset() {
    timer_stop();
    millisecond_counter = 0;
}


uint64_t millis() {
    return millisecond_counter;
}



#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer_interrupt_function() {
    millisecond_counter++;
    TA0CTL &= ~TAIFG;
}
