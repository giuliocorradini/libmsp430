#include <msp430.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum UART_speed_t {bps9600=0, bps19200, bps38400, bps57600, bps115200, n_speeds};

typedef struct __UART_t {

    //Methods

    void(*write)(const uint8_t* data, int data_len);
    void(*read)(char* buffer, int buffer_size);

} UART;

void uart_global_config(enum UART_speed_t baudrate);
void uart_init(UART* uc);

#ifdef __cplusplus
}
#endif