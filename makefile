MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

TARGET = firmware
SRCS = main.c twi.c oled.c graphics.c ui.c io.c uart.c
OBJS = $(SRCS:.c=.o)

# Arduino Uno acting as ISP settings
PORT ?= /dev/ttyUSB0
PROGRAMMER = stk500v1
BAUD = 19200

all: $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	$(SIZE) $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(TARGET).hex
	avrdude -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(TARGET).hex

fuses:
	avrdude -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0xFD:m

erase:
	avrdude -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -e

clean:
	rm -f *.o *.elf *.hex

rebuild: clean all

.PHONY: all flash fuses erase clean rebuild