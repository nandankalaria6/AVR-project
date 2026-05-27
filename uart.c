#define F_CPU 16000000UL
#include<avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include"uart.h"
#include "oled.h"

volatile char rx_buffer[32]; 
volatile uint8_t rx_index = 0;
volatile uint8_t command_ready = 0;

void uart_init()
{
    UBRR0H = 0;
    UBRR0 = 103; // baudrate 9600
    UCSR0B = (1<<TXEN0)| (1 << RXCIE0)|(1 << RXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);// 8bits, no parity
    sei();
}
// void uart_init(unsigned long baud)
// {
//     uint16_t ubrr = (F_CPU / (16UL * baud)) - 1;// baudrate 9600 according to Fcpu
//     UBRR0H = (ubrr >> 8);
//     UBRR0L = ubrr;
//     UCSR0B = (1<<TXEN0);
//     UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);// 8 bits, no parity
// }

void uart_tx(char c)
{
    while(!(UCSR0A & (1<<UDRE0))) {}
    UDR0 = c;
}
char uart_rx()
{
    while(!(UCSR0A & (1<<RXC0)));

    return UDR0;
}
void uart_print(char *s)
{
    while(*s)
    {
        uart_tx(*s);
        s++;
    }
}
ISR(USART_RX_vect)
{
    char received = UDR0;

    if(received == '\\')
    {
        rx_buffer[rx_index] = '\0';
        command_ready = 1;
        rx_index = 0;
    }
    else
    {
        if(rx_index < sizeof(rx_buffer)-1)
        {
            rx_buffer[rx_index++] = received;
        }
        if(rx_index >= sizeof(rx_buffer)-1)
        {
            rx_index = 0;
        }
    }

}
