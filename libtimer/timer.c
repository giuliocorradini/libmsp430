/*
 * timer.c
 *
 *  Created on: 07 feb 2020
 *      Author: Giulio Corradini
 */

#include <msp430.h>
#include <stdint.h>
#include <timer.h>

void __timer_default_callback() {
    ;;
}

static uint16_t __timer_count_mode = MC_0;

void (*__timer_callback_function)() = &__timer_default_callback;

void timer_config(uint16_t mode) {
    TA0CTL |= TACLR;    //Reset timer
    TA0CTL |= TASSEL_1;
    TA0CTL |= ID_0;

    TA0CTL &= ~TAIFG;
    TA0CTL |= TAIE;     //Enable interrupt

    TA0CCR0 = 0x7fff;

    __timer_count_mode = mode;
}

void timer_start() {
    TA0CTL |= __timer_count_mode;     //Timer is enabled, continuous mode
}

void timer_stop() {
    TA0CTL |= MC_0;     //Timer is halted
}

void timer_set_callback(void (*callback_function)()) {
    __timer_callback_function = callback_function;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void timer_interrupt_callback() {
    (*__timer_callback_function)();
    TA0CTL &= ~TAIFG;
}
