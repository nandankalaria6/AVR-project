#include <avr/io.h>
#include "io.h"

extern int8_t encoder_delta;

//rotary encoder sensing in main loop

void IO_init()
{
    DDRD &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6));

    PORTD |= (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5);

    DDRB &= ~((1<<PB0)|(1<<PB1));

    PORTB |= (1<<PB0)|(1<<PB1);
    //pd6 bluetooth connection
}
ButtonEvent IO_getButtonEvent()
{
    if (!(PIND & (1<<PD3))) return BTN_UP;
    if (!(PIND & (1<<PD4))) return BTN_LEFT;
    if (!(PIND & (1<<PD5))) return BTN_RIGHT;
    //if (!(PIND & (1<<PD6))) return BTN_BTCONNECT;

    return BTN_NONE;
}


void encoder_update()
{
    static uint8_t lastA = 0;

    uint8_t A = (PIND & (1<<PD2)) ? 1 : 0;

    if(A != lastA)
    {
        encoder_delta++;

        lastA = A;
    }
}
