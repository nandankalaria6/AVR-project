/*
 * il19488_driver.c
 *
 *  Created on: Jun 28, 2026
 *      Author: NANDAN
 */
#include "il19488_driver.h"


void display_command(uint8_t cmd)
{
	LCD_DC_CMD();// TELL LCD COMMAND IS COMMING
	LCD_CS_LOW();// SELECT THE LCD
	HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);// SEND COMMAND BY SPI
	LCD_CS_HIGH();// DISCONNECT FROM LCD
}
void display_data(uint8_t cmd)
{
	LCD_DC_DATA();// TELL LCD DATA IS COMMING
	LCD_CS_LOW();// SELECT THE LCD
	HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);// SEND COMMAND BY SPI
	LCD_CS_HIGH();// DISCONNECT FROM LCD
}
void display_reset()
{
	LCD_RST_LOW();
	HAL_Delay(20);// data sheet pramane 10ms hovu joi, 20 just to be safe
	LCD_RST_HIGH();
	HAL_Delay(120);// 120 ms delay to stabalise volatges;
}
void display_init(void)
{
    display_reset();//Always reset first

    //1. Positive & Negative Gamma Control (Adjusts screen contrast/brightness curves)
    display_command(0xE0);
    display_data(0x00); display_data(0x03); display_data(0x09); display_data(0x08);
    display_data(0x16); display_data(0x0A); display_data(0x3F); display_data(0x78);
    display_data(0x4C); display_data(0x09); display_data(0x0A); display_data(0x08);
    display_data(0x16); display_data(0x1A); display_data(0x0F);

    display_command(0xE1);
    display_data(0x00); display_data(0x16); display_data(0x19); display_data(0x03);
    display_data(0x0F); display_data(0x05); display_data(0x32); display_data(0x45);
    display_data(0x46); display_data(0x04); display_data(0x0E); display_data(0x0D);
    display_data(0x35); display_data(0x37); display_data(0x0F);

    //2. Power Control (Sets up stable internal driver voltages)
    display_command(0xC0);//Power Control 1
    display_data(0x17);//Vreg1out
    display_data(0x15);//Vg2out

    display_command(0xC1);//Power Control 2
    display_data(0x41);//VGH, VGL driving voltages

    display_command(0xC5);//VCOM Control(prevents display flickering)
    display_data(0x00); display_data(0x12); display_data(0x80);

    //3. Memory Access Control (Sets screen orientation and color layout)
    display_command(0x36);
    display_data(0x48);//0x48 sets standard portrait mode with RGB pixel order

    //4. Interface Pixel Format (CRITICAL: Sets color requirements over SPI)
    display_command(0x3A);
    display_data(0x66);//0x66 forces 18-bit color mode (3 bytes per pixel)

    display_command(0xB0);//Interface Mode Control
    display_data(0x00);

    display_command(0xB1);//frame Rate Control
    display_data(0xA0);//sets screen refresh rate to roughly 60Hz

    display_command(0xB4);//display Inversion Control
    display_data(0x02);//2-dot inversion configuration

    display_command(0xE9);//set Image Function
    display_data(0x00);//disables the wide 24-bit data bus

    //5. Wake up display engine
    display_command(0x11);//exit sleep mode
    HAL_Delay(120);//must wait 120ms before sending next command

    display_command(0x29);//display ON command
}


void display_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    display_command(0x2A);// Tell the screen the column boundaries (Left X to Right X)
    display_data(x0 >> 8);// Send high byte of starting X
    display_data(x0 & 0xFF);// Send low byte of starting X
    display_data(x1 >> 8);// Send high byte of ending X
    display_data(x1 & 0xFF);// Send low byte of ending X

    // Tell the screen the row boundaries (Top Y to Bottom Y)
    display_command(0x2B);
    display_data(y0 >> 8);// Send high byte of starting Y
    display_data(y0 & 0xFF);// Send low byte of starting Y
    display_data(y1 >> 8);// Send high byte of ending Y
    display_data(y1 & 0xFF);// Send low byte of ending Y
    // Prepare internal memory to receive pixel colors
    display_command(0x2C);//RAM Write Command
}

void display_write_buffer(uint8_t *buffer, uint32_t size)
{
	//cant make buffer for whole screen in ram so we send data in 'bursts'
	LCD_DC_DATA();
	LCD_CS_LOW();
	HAL_SPI_Transmit(&hspi1, buffer, size, HAL_MAX_DELAY);;
	LCD_CS_HIGH();
}

void display__clear(uint16_t colour565)
{
	display_set_window(0, 0, 319, 479);//320x480
	uint8_t row_buffer[960];//320*3 each for rgb

    //Convert 16-bit RGB565 color to 24-bit RGB888 format
    uint8_t r = (color565 >> 11) << 3;//Extract Red (5 bits) and scale up to 8 bits
    uint8_t g = ((color565 >> 5) & 0x3F) << 2;//Extract Green (6 bits) and scale up to 8 bits
    uint8_t b = (color565 & 0x1F) << 3;//Extract Blue (5 bits) and scale up to 8 bits

    for(int i = 0; i < 960; i += 3)
    {
        row_buffer[i]     = r;//Red byte
        row_buffer[i + 1] = g;//Green byte
        row_buffer[i + 2] = b;//Blue byte
    }
    for(int i=0;i<480;i++)
    {
    	display_write_buffer(row_buffer, 960);// sending it 480 times
    }

}
void display_draw_pixel(uint16_t x, uint16_t y, uint16_t color565)
{
    if(x >= 320 || y >= 480) return;

    display_set_window(x, y, x, y);// create new window for that pixel

    // 2. Convert the 16-bit color to 24-bit color bytes(565 to 888)
    uint8_t pixel_bytes[3];
    pixel_bytes[0] = (color565 >> 11) << 3;         // Red
    pixel_bytes[1] = ((color565 >> 5) & 0x3F) << 2; // Green
    pixel_bytes[2] = (color565 & 0x1F) << 3;        // Blue

    display_write_buffer(pixel_bytes, 3);
}
