#ifndef TIMER_H
#define TIMER_H
#include <msp430.h>
#include <stdint.h>

void timer_init();
void timer_reset();
void timer_start();
void timer_stop();
void timer_set_counting_mode(uint16_t cm);
void timer_set_compare_value(uint16_t cv);
void timer_reset_millis();
void wait(int ms);
uint32_t millis();
uint32_t minutes();

#endif
