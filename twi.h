#ifndef TWI_H
#define TWI_H

#include <stdint.h>

void TWI_init();
void TWI_start();
void TWI_write(uint8_t data);
void TWI_stop();

#endif