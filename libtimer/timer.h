/*
 * timer.h
 *
 *  Created on: 07 feb 2020
 *      Author: Giulio Corradini
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#define UP_MODE MC_1;
#define CONTINUOS_MODE  MC_2;
#define UP_DOWN_MODE    MC_3;

void timer_config(uint16_t mode);
void timer_set_callback();
void timer_start();
void timer_stop();


#endif /* TIMER_H_ */
