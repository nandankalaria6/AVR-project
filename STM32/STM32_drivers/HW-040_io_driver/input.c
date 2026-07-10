/*
 * input.c
 *
 *  Created on: Jun 29, 2026
 *      Author: NANDAN
 */

#include "main.h"
#include "input.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2; // timer 2 selected
static volatile bool button_triggered = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin ==ENCODER_SW_Pin)
    {
        button_triggered = true;
    }
    if (GPIO_Pin == ENCODER_SW_Pin) {
            button_triggered = true;
            // Temporary test: Toggle the Blue Pill's onboard LED (usually PC13)
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }
}

void Input_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
}


void Input_Update(Encoder_State_t *state) {
    static uint32_t last_debounce_time = 0;
    // encoder nu reading
    uint32_t current_time = HAL_GetTick();
    state->current_pos = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);

    //button nu debouncing
    state->button_pressed = false; // Reset the state default
    if (button_triggered)
    {
        button_triggered = false; // Reset the interrupt flag
        if ((current_time - last_debounce_time) > DEBOUNCE_DELAY_MS)// agar 50ms thi jau reye che to bounce nathi, actual click che
        {
        	if (HAL_GPIO_ReadPin(ENCODER_SW_GPIO_Port, ENCODER_SW_Pin) == GPIO_PIN_RESET)//Double check
            {
                state->button_pressed = true;
                last_debounce_time = current_time;
            }
        }
    }
}
