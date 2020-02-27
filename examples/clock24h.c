#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include "display.h"
#include "timer.h"


/**
 * clock24.h
 *
 * 24 hours wall clock
 * Uses Timer A and FSS Display
 *
 */

uint32_t offset = 0;    //Initial hour in minutes
uint32_t mins;
uint16_t display;

char current_digit;
char dot = 0;

bool configuring = false;
bool next = false;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	/*
	 * GPIO configuration
	 */

	//GPIO Configuration
    P6DIR |= 0x0F;
    P3DIR |= 0xF0 | BIT2;

    P4DIR |= (BIT0 | BIT1 | BIT2 | BIT3);
    P1DIR |= (BIT2 | BIT3 | BIT4 | BIT5);

    P4OUT = 0x00;
    P1OUT = 0x00;

    //Switch 1: data
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;
    P2IES |= BIT1;
    P2IE |= BIT1;

    //Switch 2 configuration
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IES |= BIT1;
    P1IE |= BIT1;

    __enable_interrupt();

    timer_init();
    timer_start();

    while(1) {

        if(configuring) {

            display &= ~(0x0f << 4*(dot-1));
            display |= current_digit << 4*(dot-1);

            if(next && dot==4) {
                next = false;
                configuring = false;
                offset = display & 0x000f;
                offset += ((display & 0x00f0) >> 4 ) * 10;
                offset += ((display & 0x0f00) >> 8 ) * 60;
                offset += ((display & 0xf000) >> 12 ) * 600;
                timer_reset_millis();
            } else if (next) {
                next = false;
                dot++;
                current_digit = display & (0x0f << 4 * (dot-1));
            }

        } else {

            dot = millis() / 1000 % 2 * 3;
            mins = minutes() + offset;

            display = mins % 10;
            display += (mins / 10 % 6) << 4;

            int hours = mins / 60 % 24;
            display += hours % 10 << 8;
            display += hours / 10 % 3 << 12;

        }


        display_show_value(display, dot, 10, DISPLAY_COLOR_RED);
    }

	return 0;
}


//Switch 1
#pragma vector = PORT2_VECTOR
__interrupt void p2isr() {

    int i;
    unsigned long int delay;
    for(i=35; i>0; i--)
                    for(delay=10000000; delay>0; delay--);;

    if(!(P2IN & BIT1)) {
        if(!configuring) {
            configuring = true;
            dot = 1;
            current_digit = display & 0x0f;
        } else {
            next = true;
        }
    }

    P2IFG &= ~BIT1;
}

//Switch 2
#pragma vector = PORT1_VECTOR
__interrupt void p1isr() {

    int i;
    unsigned long int delay;
    for(i=35; i>0; i--)
                for(delay=10000000; delay>0; delay--);;

    if(!(P1IN & BIT1)) {
        if(configuring) {
            current_digit++;
            if(dot==2)
                current_digit %= 6;
            else if(dot==4 && (display & 0x0f00) > 0x0300)
                current_digit %= 2;
            else if(dot==4)
                current_digit %= 3;
            else
                current_digit %= 10;
        }
    }

    P1IFG &= ~BIT1;
}
