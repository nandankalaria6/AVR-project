#ifndef OLED_H
#define OLED_H

#include <stdint.h>

void OLED_update();
void OLED_init();
void OLED_command(uint8_t cmd);
void OLED_data(uint8_t data);
void OLED_fill(uint8_t pattern);

#endif