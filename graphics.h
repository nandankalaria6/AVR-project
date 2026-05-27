#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

extern uint8_t buffer[1024];

void clearBuffer();
void drawPixel(uint8_t x, uint8_t y, uint8_t color);
void drawHLine(uint8_t x, uint8_t y, uint8_t len);
void drawVLine(uint8_t x, uint8_t y, uint8_t len);
void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void drawString(uint8_t x, uint8_t y, char *str);
void drawChar(uint8_t x, uint8_t y, char c);

#endif