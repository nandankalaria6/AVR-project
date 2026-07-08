/*
 * display_driver.c
 *
 *  Created on: Jun 19, 2026
 *      Author: NANDAN
 */
#include "ssd1306_driver.h"
#include "graphics.h"

extern I2C_HandleTypeDef hi2c1;


void display_command(uint8_t cmd)
{
	uint8_t i2c_buffer[2];
	i2c_buffer[0]=0x00;
	i2c_buffer[1]=cmd;
	HAL_I2C_Master_Transmit(&hi2c1, 0x78,i2c_buffer,2,10);
}

void display_init()
{
	display_command(0xAE);// display off set kairu
	display_command(0xD5);//set display clock
	display_command(0x80);
	display_command(0xA8);
	display_command(0x3F);

	display_command(0xD3);
	display_command(0x00);

	display_command(0x40);

	display_command(0x8D);
	display_command(0x14);

	display_command(0x20);
	display_command(0x00);

	display_command(0xA1);

	display_command(0xC8);

	display_command(0xDA);
	display_command(0x12);

	display_command(0x81);
	display_command(0xCF);

	display_command(0xD9);
	display_command(0xF1);

	display_command(0xDB);
	display_command(0x40);

	display_command(0xA4);

	display_command(0xA6);

	display_command(0xAF);
}

void display_setCursor(uint8_t page,uint8_t column)
{
	display_command(0xB0 | page);
	display_command(0x00 | (column & 0x0F));
	display_command(0x10 | ((column >>4)& 0x0F));
}

void display_data(uint8_t data)
{
	uint8_t i2c_buffer[2];
	i2c_buffer[0]=0x40;
	i2c_buffer[1]=data;
	HAL_I2C_Master_Transmit(&hi2c1, 0x78,i2c_buffer,2,10);
}
void display_fill(uint8_t pattern)
{
    for(uint8_t page = 0; page < 8; page++)
    {
    	display_setCursor(page, 0);

        for(uint8_t col = 0; col < 128; col++)
        {
        	display_data(pattern);
        }
    }
    // fill the screen 8 pixels at a time;
}

void display_update()
{
    static uint8_t temp_buffer[129];
	temp_buffer[0]=0x40;
	for(uint8_t page=0; page<8;page++)
    {
		display_setCursor(page, 0);
    	for(int i=1;i<129;i++)temp_buffer[i]=buffer[page*128+i-1];
    	HAL_I2C_Master_Transmit(&hi2c1, 0x78,temp_buffer,129,50);
		//for(int i=0;i<128;i++) display_data(buffer[page*128+i]);
    }

}
