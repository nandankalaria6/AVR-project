#include <avr/io.h>
#include <stdint.h>

#include "twi.h"
#include "oled.h"
#include "graphics.h"

void OLED_command(uint8_t cmd)
{
    TWI_start();
    TWI_write(0x78);
    TWI_write(0x00);// says that next bytes will be a command
    TWI_write(cmd);
}
void OLED_init()
{
    OLED_command(0xAE);

    OLED_command(0xD5);
    OLED_command(0x80);

    OLED_command(0xA8);
    OLED_command(0x3F);

    OLED_command(0xD3);
    OLED_command(0x00);

    OLED_command(0x40);

    OLED_command(0x8D);
    OLED_command(0x14);

    OLED_command(0x20);
    OLED_command(0x00);

    OLED_command(0xA1);

    OLED_command(0xC8);

    OLED_command(0xDA);
    OLED_command(0x12);

    OLED_command(0x81);
    OLED_command(0xCF);

    OLED_command(0xD9);
    OLED_command(0xF1);

    OLED_command(0xDB);
    OLED_command(0x40);

    OLED_command(0xA4);

    OLED_command(0xA6);

    OLED_command(0xAF);
    /*
    Here these are set up
    charge pump
    multiplex ratio
    COM scan direction
    memory addressing mode
    contrast settings
    display RAM config*/
}
void OLED_data(uint8_t data)
{
    TWI_start();
    TWI_write(0x78);//address of oled
    TWI_write(0x40);// display RAM data
    TWI_write(data);
    TWI_stop();
}
void OLED_setCursor(uint8_t page,uint8_t column)
{
    OLED_command(0xB0 | page);
    OLED_command(0x00 | (column & 0x0F));
    OLED_command(0x10 | ((column >>4)& 0x0F));
}
void OLED_fill(uint8_t pattern)
{
    for(uint8_t page = 0; page < 8; page++)
    {
        OLED_setCursor(page, 0);

        for(uint8_t col = 0; col < 128; col++)
        {
            OLED_data(pattern);
        }
    }
    // fill the screen 8 pixels at a time;
}
void OLED_update()
{
    for(uint8_t page=0; page<8;page++)
    {
        OLED_setCursor(page,0);
        TWI_start();
        TWI_write(0x78);//address of oled 
        TWI_write(0x40);//display RAM data
        for(uint8_t coloumn=0; coloumn<128;coloumn++)
        {
            TWI_write(buffer[page*128 + coloumn]);
        }
        TWI_stop();
    }
}
