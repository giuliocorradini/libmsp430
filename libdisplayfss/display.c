/*
 * display.c
 *
 *  Created on: 13 gen 2020
 *      Author: Giulio Corradini
 */
#include <msp430.h>
#include <stdint.h>
#include <display.h>

static number num2seg[16] = {
                         {.msb = 0x30, .lsb = 0x0F},
                         {.msb = 0x00, .lsb = 0x06},
                         {.msb = 0x50, .lsb = 0x0B},
                         {.msb = 0x40, .lsb = 0x0F},
                         {.msb = 0x60, .lsb = 0x06},
                         {.msb = 0x60, .lsb = 0x0D},
                         {.msb = 0x70, .lsb = 0x0D},
                         {.msb = 0x00, .lsb = 0x07},
                         {.msb = 0x70, .lsb = 0x0F},
                         {.msb = 0x60, .lsb = 0x0F},
                         {.msb = 0x70, .lsb = 0x07},
                         {.msb = 0x70, .lsb = 0x0C},
                         {.msb = 0x30, .lsb = 0x09},
                         {.msb = 0x50, .lsb = 0x0E},
                         {.msb = 0x70, .lsb = 0x09},
                         {.msb = 0x70, .lsb = 0x01},
};

extern display_pin_t display_pins[D_TOT_NUM] = {//size is D_TOT_NUM

                                 {.port = &P4OUT, .pin = BIT0},
                                 {.port = &P4OUT, .pin = BIT1},
                                 {.port = &P4OUT, .pin = BIT2},
                                 {.port = &P4OUT, .pin = BIT3},

                                 //Green
                                 {.port = &P1OUT, .pin = BIT2},
                                 {.port = &P1OUT, .pin = BIT3},
                                 {.port = &P1OUT, .pin = BIT4},
                                 {.port = &P1OUT, .pin = BIT5}


};

void display_set_pin(uint8_t display, volatile unsigned char* port, uint8_t bit) {
    if(display<D_TOT_NUM){
        display_pins[display].port = port;
        display_pins[display].pin = bit;
    }
}

void display_shutdown_display(uint8_t disp) {

    *display_pins[disp].port &= ~(display_pins[disp].pin);
}

void display_show_raw(uint8_t display, uint8_t raw) {
    if(display<D_TOT_NUM) {
        //Flush display
        P6OUT &= 0xf0;
        P3OUT &= 0x0f;

        display_pin_t selected_display = display_pins[display];
        *(selected_display.port) |= selected_display.pin;


        P6OUT = raw & 0x0f;   //selects LSB
        P3OUT = raw & 0xf0;   //selects MSB

    }
}

void display_show_digit(uint8_t display, uint8_t value, uint8_t dot) {
    number num = num2seg[value];
    uint8_t raw = num.lsb + num.msb | dot;
    display_show_raw(display, raw);
}

void display_show_value(uint16_t value, uint8_t dot_pos, uint32_t cycles, uint8_t color) {
    //This is a blocking function
    //1 unit of cycles is roughly 20 ms
    uint32_t cycle_count;
    int8_t current_display;

    for(cycle_count = cycles; cycle_count > 0; cycle_count--) {
        for(current_display = 0; current_display < 4; current_display++) {
            uint8_t digit = (value >> (4 * current_display)) & 0x0f;

            if((dot_pos-1) == (current_display)) {
                display_show_digit(current_display+color, digit, DISPLAY_SHOW_DOT);
            } else {
                display_show_digit(current_display+color, digit, DISPLAY_NO_DOT);
            }
            __delay_cycles(5000);
            display_shutdown_display(current_display + color);
        }
    }

}

void display_show_number(int16_t number, uint8_t dot_pos, uint32_t cycles, uint8_t color) {
    uint32_t cycle_count;
    int8_t current_display;
    int16_t m;

    if(number >= -999 && number <= 9999) {

        for(cycle_count = cycles; cycle_count > 0; cycle_count--) {
            for(current_display = 0, m=1; current_display < 4; current_display++, m*=10) {
                if(current_display == 3 && number < 0) {
                    display_show_raw(current_display, DISPLAY_MINUS_SIGN);
                } else {

                    int8_t digit = abs((number / m) % 10);

                    if((dot_pos-1) == current_display) {
                        display_show_digit(current_display+color, digit, DISPLAY_SHOW_DOT);
                    } else {
                        display_show_digit(current_display+color, digit, DISPLAY_NO_DOT);
                    }

                }

                __delay_cycles(5000);
                display_shutdown_display(current_display + color);
            }
        }

    }
}

void display_show_yellow_value(uint16_t value, uint8_t dot_pos, uint32_t cycles) {

    uint32_t cycle_count;
    int8_t current_display;
    int8_t toggler = 0x00;

    for(cycle_count = cycles / 2; cycle_count > 0; cycle_count--) {
        if(toggler){
            for(current_display = 0; current_display < 4; current_display++) {
                uint8_t digit = (value >> (4 * current_display)) & 0x0f;

                if((dot_pos-1) == (current_display)) {
                    display_show_digit(current_display, digit, DISPLAY_SHOW_DOT);
                } else {
                    display_show_digit(current_display, digit, DISPLAY_NO_DOT);
                }
                __delay_cycles(2500);
                display_shutdown_display(current_display);
            }
        } else {
            for(current_display = 0; current_display < 4; current_display++) {
                uint8_t digit = (value >> (4 * current_display)) & 0x0f;

                if((dot_pos-1) == (current_display)) {
                    display_show_digit(current_display+DISPLAY_COLOR_GREEN, digit, DISPLAY_SHOW_DOT);
                } else {
                    display_show_digit(current_display+DISPLAY_COLOR_GREEN, digit, DISPLAY_NO_DOT);
                }
                __delay_cycles(2500);
                display_shutdown_display(current_display + DISPLAY_COLOR_GREEN);
            }
        }
        toggler ^= 0x01;
    }

}

void display_show_text(char* text, int tl, uint32_t cycles, uint8_t color) {
    //Total time is cycles * text length
    uint32_t cycle_count;
    int8_t current_display;
    int8_t curr_lett;

    for(curr_lett = 0; curr_lett < tl; curr_lett++) {

        for(cycle_count = cycles; cycle_count > 0; cycle_count--) {
            for(current_display = 0; current_display < 4; current_display++) {

                uint8_t lett = text[(curr_lett+current_display) % tl];

                display_show_raw(current_display+color, lett);

                __delay_cycles(5000);
                display_shutdown_display(current_display + color);
            }
        }

    }
}
