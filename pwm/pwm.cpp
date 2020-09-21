#include "pwm.h"
#include "timer.h"
using namespace timer;

PWM::PWM(float duty_cycle, float frequency) {
    this->duty_cycle = duty_cycle;
    this->frequency = frequency;

    //Enable P1.2 for output
    P1DIR |= BIT2;

    TA0CCR0 = 32768/frequency - 1;

    TA0CCTL1 |= OUTMOD_7;
    TA0CCR1 = duty_cycle * 32768/frequency;
}

void PWM::start() {
    P1SEL |= BIT2;  //Attach Timer A0 to output pin
    TA0CTL = TACLR + TASSEL_1 + MC__UP;
}

void PWM::stop() {
    P1SEL &= ~BIT2; //Detach Timer A0
    P1OUT &= ~BIT0; //Set output to low value
    TA0CTL |= MC__STOP;
}

void PWM::set_frequency(float new_frequency) {
    frequency = new_frequency;
    TA0CCR0 = 32768/frequency - 1;
    TA0CCR1 = duty_cycle * frequency;
}

void PWM::set_duty_cycle(float new_duty_cycle) {
    duty_cycle = new_duty_cycle;
    TA0CCR1 = duty_cycle * 32768/frequency;
}

void PWM::play(float freq, int time) {
    set_frequency(freq);
    start();
    wait(time);
}

void PWM::play(int freq, int time) {
    play((float)freq, time);
}

void PWM::mute(int time) {
    stop();
    wait(time);
}