#ifndef PWM_H
#define PWM_H
#endif

#include <timer.h>

namespace timer {

class PWM {
    private:
        float duty_cycle;
        float frequency;
    public:
        PWM(float duty_cycle=0.5, float frequency=440.0);
        void start();
        void stop();
        void set_frequency(float new_f);
        void set_duty_cycle(float new_dc);
        void play(float freq, int time);
        void play(int freq, int time);
        void mute(int time);
};

}