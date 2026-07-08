//#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
//-DF_CPU=8000000UL
#define F_CPU 8000000UL
//avr-gcc -mmcu=atmega328p -DF_CPU=8000000UL -Os -o i2c.elf i2c.c
//avr-objcopy -O ihex -R .eeprom i2c.elf i2c.hex
/* avrdude \
-C /etc/avrdude.conf \
-v \
-p atmega328p \
-c stk500v1 \
-P /dev/ttyUSB0 \
-b 19200 \
-U flash:w:i2c.hex:i
*/


void TWI_init()
{
    TWSR = 0x00;
    TWBR=72; // i2c bitrate=50kHz
    PORTC |= (1<<PC4) | (1<<PC5); // internal pullu
}
void TWI_start()
{
    TWCR=(1<<TWINT)| (1<<TWSTA) | (1<<TWEN);// twint=>interupt flag, TWsta=> generate 'start',TWEN=> hardware ma enable kairu i2c
    while(!(TWCR & (1<<TWINT))); //wait till interupt becomes 0;
}
void TWI_write(uint8_t data)
{
    TWDR=data; // twdr ma data naikhu
    TWCR = (1<<TWINT) | (1<<TWEN);// transmision triggered
    while(!(TWCR & (1<<TWINT)));// wait for completion
}
void TWI_stop()
{
    TWCR = (1<<TWINT)| (1<<TWEN)| (1<<TWSTO);// stop bit on kari nakhyo
}
uint8_t TWI_status()
{
    return (TWSR & 0xF8);
    //TWSR stores status...., upper 5 bits store current status
    
    // after start expected 0x08(started) 
    //after connection 0x18 (slave acknowldgement address)
    // after data recieved 0x28(slave acknowledge data)
}
