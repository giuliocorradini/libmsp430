/*
 * timer.c
 *
 *  Created on: 24 gen 2020
 *      Author: Giulio Corradini
 */

#include <timer.h>
#include <stdint.h>

static volatile uint32_t millis_counter = 0;

void timer_init() {


    __enable_interrupt();

}

void timer_start() {
    TA2CTL = TASSEL_1 + ID_0 + MC__UP;
    TA2CCTL0 |= CCIE;
    TA2CCR0 = 33;

    __enable_interrupt();
}

long int millis() {
    return millis_counter;
}

void __attribute__((optimize("O0"))) wait(uint32_t ms) {
    uint32_t waiting_init_time = millis();
    while(millis() < waiting_init_time+ms);;
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void timer_A0_capcomp_isr(void) {
    millis_counter++;
	TA2CCTL0 &= ~CCIFG;
}
