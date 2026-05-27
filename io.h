#ifndef IO_H
#define IO_H

#include <stdint.h>

typedef enum
{
    BTN_NONE,
    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_RIGHT
} ButtonEvent;

void IO_init();
ButtonEvent IO_getButtonEvent();
void encoder_update();

#endif