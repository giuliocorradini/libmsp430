#include <msp430.h>
#include <stdint.h>
#include "display.h"
#include "timer.h"

/**
 * stopwatch.c
 *
 * Stopwatch using libdisplay-fss and libtimer
 *
 */
char running = 0;

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

	while(1) {

	    uint32_t elapsed_time = millis() / 100;

	    uint16_t display = elapsed_time % 10;
	    display += (elapsed_time / 10 % 10) << 4;
	    display += (elapsed_time / 100 % 6) << 8;
	    display += (elapsed_time / 600 % 10) << 12;

	    display_show_value(display, DISPLAY_NO_DOT, 2, DISPLAY_COLOR_RED);
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

        if(running) {
            timer_stop();
            running = 0;
        } else {
            timer_start();
            running = 1;
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

        timer_stop();
        timer_reset_millis();
        running = 0;

    }

    P1IFG &= ~BIT1;
}
