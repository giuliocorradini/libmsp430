#ifndef MSP430_H
#include <msp430.h>
#endif
#ifndef STDINT_H
#include <stdint.h>
#endif

//Dot constants for display_show_single_value
#define DISPLAY_NO_DOT  0x00
#define DISPLAY_SHOW_DOT    0x80

//Cycles for display_show_value
#define DISPLAY_ENDLESS_LOOP    0xFFFFFFFF
#define DISPLAY_ONCE    0x00000000

void display_shutdown_display(uint8_t display_num);
void display_show_raw(uint8_t display, uint8_t raw);
void display_show_digit(uint8_t display_num, uint8_t digit, uint8_t dot);
void display_show_value(uint16_t value, uint8_t dot_pos, uint32_t cycles, uint8_t color);
void display_show_number(int16_t number, uint8_t dot_pos, uint32_t cycles, uint8_t color); //from -999 to 9999

void display_show_text(char* text, int tl, uint32_t cycles, uint8_t color);

//Letters for show text
#define dl_BLANK    0x00
#define dl_A    0x77
#define dl_B    0x7c
#define dl_C    0x39
#define dl_D    0x5e
#define dl_E    0x79
#define dl_F    0x71
#define dl_G    0x3d
#define dl_H    0x74
#define dl_I    0x30
#define dl_J    0x1e
#define dl_L    0x38
#define dl_N    0x37
#define dl_O    0x3f
#define dl_P    0x72
#define dl_Q    0x67
#define dl_R    0x33
#define dl_S    0x6d
#define dl_T    0x78
#define dl_U    0x3e
#define dl_Y    0x6e
#define dl_Z    0x5b
#define dl_EXCLAMATION_MARK 0x82

void display_show_yellow_value(uint16_t value, uint8_t dot_pos, uint32_t cycles);

enum display_type_t {D1_RED=0, D2_RED, D3_RED, D4_RED, D1_GREEN, D2_GREEN, D3_GREEN, D4_GREEN, D_TOT_NUM};

#define DISPLAY_COLOR_RED D1_RED
#define DISPLAY_COLOR_GREEN D1_GREEN

#define DISPLAY_MINUS_SIGN  0x40

typedef struct _display {
    volatile unsigned char* port;
    uint8_t pin;
} display_pin_t;

typedef struct _number {
    uint8_t msb;
    uint8_t lsb;
} number;
