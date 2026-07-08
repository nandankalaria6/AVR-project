#ifndef DH11_H
#define DH11_H

#include <stdbool.h>
#include <stdint.h>

uint8_t read_bit(void);
uint8_t read_byte(void);
bool read_dh11(uint8_t* humidity_int,uint8_t* humidity_dec,uint8_t* temp_int, uint8_t* temp_dec,uint8_t* checksum);

#endif
