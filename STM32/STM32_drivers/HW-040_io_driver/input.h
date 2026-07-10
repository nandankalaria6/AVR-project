/*
 * input.h
 *
 *  Created on: Jun 29, 2026
 *      Author: NANDAN
 */

#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>
#define DEBOUNCE_DELAY_MS 20
typedef struct {
    int16_t current_pos;
    int16_t last_pos;
    bool button_pressed;
} Encoder_State_t;

void Input_Init(void);
void Input_Update(Encoder_State_t *state);

#endif /* INPUT_H */
