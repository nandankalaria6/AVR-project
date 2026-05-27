#ifndef UART_H
#define UART_H
#include <util/delay.h>
#include <avr/interrupt.h>
void uart_init();
void uart_tx(char c);
void uart_print(char *s);
char uart_rx();
extern volatile char rx_buffer[32];
extern volatile uint8_t command_ready;
extern volatile uint8_t rx_index;
#endif