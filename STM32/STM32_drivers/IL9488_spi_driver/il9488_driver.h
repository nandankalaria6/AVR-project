/*
 * il19488_driver.h
 *
 *  Created on: Jun 28, 2026
 *      Author: NANDAN
 */

#ifndef SRC_IL19488_DRIVER_H_
#define SRC_IL19488_DRIVER_H_
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;

#define LCD_CS_LOW()    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_HIGH()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

#define LCD_DC_CMD()    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)
#define LCD_DC_DATA()   HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)

#define LCD_RST_LOW()   HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define LCD_RST_HIGH()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

void display_command(uint8_t cmd);
void display_data(uint8_t cmd);
void display_reset(void);
void display_init(void);
void display_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void display_write_buffer(uint8_t *buffer, uint32_t size);
void display_clear(uint16_t color565);
void display_draw_pixel(uint16_t x, uint16_t y, uint16_t color565);

#endif /* SRC_IL19488_DRIVER_H_ */
