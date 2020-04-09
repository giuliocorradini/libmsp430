/*
 * timer.c
 *
 *  Created on: 24 gen 2020
 *      Author: Giulio Corradini
 */

#include <msp430.h>
#include <stdint.h>
#include <timer.h>

static uint16_t timer_counting_mode = MC_1;     //Timer counting mode is Up/Down
static uint32_t timer_millis_counter = 0;

void timer_init() {
    TA0CTL |= TACLR;    //Reset timer


    TA0CTL = TASSEL_2 + ID_0 + timer_counting_mode; //Select SMCLK as clock source and enable interrupts

    __enable_interrupt();

    TA0CCR0 = 1050;     //Counts up to 1000 in order to have milliseconds interrupt
}

void timer_start() {
    TA0CCTL0 |= CCIE;
}

void timer_reset() {
    TA0CTL |= TACLR;
    timer_millis_counter = 0;
}

void timer_stop() {
    TA0CCTL0 &= ~CCIE;
}

void timer_reset_millis() {
    timer_millis_counter = 0;
}

void timer_set_counting_mode(uint16_t cm) {
    timer_counting_mode = cm & 0x0030;
}

void timer_set_compare_value(uint16_t cv) {
    TA0CCR0 = cv;
}

uint32_t millis() {
    return timer_millis_counter;
}

uint32_t minutes() {
    return timer_millis_counter / 60000;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void int_callback_millis_count(void) {
    timer_millis_counter++;
    TA0CTL &= ~TAIE;
}

void wait(int ms) {
    int init_time = millis();
    int curr_time = init_time;

    do {
        curr_time = millis();
    } while (curr_time - init_time < ms);
}
