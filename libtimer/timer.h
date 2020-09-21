#ifndef TIMER_H
#define TIMER_H
#endif

#ifdef __cplusplus
extern "C" {
#endif


#include <msp430.h>
#include <stdint.h>

void timer_init();
void timer_start();
long int millis();
void wait(uint32_t ms);


#ifdef __cplusplus
}
#endif