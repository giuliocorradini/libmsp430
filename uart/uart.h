#ifndef __uart_h
#define __uart_h

#include <msp430.h>
#include <stdint.h>
#include "circular_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

enum UART_speed_t {bps9600=0, bps19200, bps38400, bps57600, bps115200, n_speeds};

void uart_config(enum UART_speed_t baudrate);

void uart_putchar(char ch);
char uart_getchar();

void uart_write(const char* src, int n);
void uart_writeline(const char* str);

void uart_read(char* dst, int n);
int uart_async_read(char* dst, int maxn);

#ifdef __cplusplus
}
#endif

#endif