#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "twi.h"
#include "oled.h"
#include "graphics.h"
#include "ui.h"
#include "io.h"
#include "uart.h"



int8_t encoder_delta = 0;
volatile uint32_t randomSeed = 0;
int main()
{
    DDRC |= (1 << PC0);
    currentScreen = SCREEN_HOME;
    command_ready=0;
    rx_index=0;

    TWI_init();
    OLED_init();
    IO_init();
        clearBuffer();
    drawString(0,0,"WAIT");
    OLED_update();
    uart_init();
    sei();


    Screen PrevScreen=SCREEN_HOME;
while(1)
{    
    randomSeed++;
    encoder_update();
    if(command_ready)
        {

            if(rx_buffer[2]=='T')
            {
                memcpy(currentTime, &rx_buffer[3], 5);
                currentTime[5] = '\0';
                memcpy(currentDate, &rx_buffer[9], 8);
                currentDate[8] = '\0';
                // the date and time gets parsed parsed is copied to CurrentTine and CurrentDate 
            }
            OLED_update();
            command_ready = 0;
        }
    
    if(currentScreen==SCREEN_MENU)
    {
        if(encoder_delta >= 3)
        {
            selectedItem++;
            if(selectedItem >= MENU_SIZE) selectedItem = 0;
            encoder_delta = 0;
        }
        if(encoder_delta <= -3)
        {
            selectedItem--;
            if(selectedItem < 0) selectedItem = MENU_SIZE - 1;
            encoder_delta = 0;
        }
    }

 if(currentScreen==SCREEN_TOOLS)
    {
        if(encoder_delta >= 3)
        {
            toolSelected++;
            if(toolSelected >= TOOL_SIZE) toolSelected=0;
            encoder_delta=0;
        }
        if(encoder_delta <= -3)
        {
            toolSelected--;
            if(toolSelected < 0) toolSelected=TOOL_SIZE - 1;
            encoder_delta = 0;
        }
    }


    ButtonEvent btn = IO_getButtonEvent();

    if(btn == BTN_RIGHT && currentScreen == SCREEN_MENU)
    {
        PrevScreen=currentScreen;
        switch(selectedItem)
        {
            case 0:                
                currentScreen = SCREEN_TOOLS;
                break;

            case 1:
                currentScreen = SCREEN_TEMP;
                break;

            case 2:
                currentScreen = SCREEN_SETTINGS;
                break;
            case 3:
                currentScreen = SCREEN_DINO;
                break;

        }
    }
        if(stopwatchRunning)
    {
        stopwatchMs += 103;
    }

    if(btn == BTN_RIGHT && currentScreen == SCREEN_TOOLS)
    {
        PrevScreen=currentScreen;
        switch(toolSelected)
        {
            case 0:
                currentScreen = SCREEN_STOPWATCH;
                break;
            case 1:
                currentScreen = SCREEN_DICE;
                break;
            case 2:
                currentScreen = SCREEN_SETTINGS;
                break;
            case 3:
                currentScreen = SCREEN_DINO;
                break;
        }
    }
    if(currentScreen == SCREEN_STOPWATCH && btn!= BTN_LEFT)
    {
        if(btn == BTN_RIGHT)
        {
            stopwatchRunning = !stopwatchRunning;
        }
        if(btn == BTN_UP)
        {
            stopwatchMs = 0;
        }
    }
    if(currentScreen == SCREEN_DINO)
    {
        if(btn == BTN_UP && !isJumping)
        {
            velocityY = -12;
            isJumping = 1;// makes dino jump
            drawPixel(7,47,1);
            drawPixel(19,48,1);// particle effects
        }
    }
    if(currentScreen == SCREEN_DINO && gameOver)
    {
        if(btn == BTN_RIGHT)
        {
            gameOver = 0;
            score = 0;
            gameSpeed = 2;
            cactusX = 128;
            dinoY = 42;
            velocityY = 0;
            isJumping = 0;
            cactusType = 0;
            gameTick=0;
        }
        // restart game
    }
    if(currentScreen == SCREEN_DICE)
    {
        if(btn == BTN_RIGHT)
        {
            for(uint8_t i=0;i<10;i++)
            {
                diceValue++;

                if(diceValue > 20)
                {
                    diceValue = 1;
                }
                UI_render();
                _delay_ms(10);
            }
                diceValue=(randomSeed%20)+1;
                UI_render();
        }
    }


    if(btn== BTN_RIGHT && currentScreen==SCREEN_HOME)
    {
        PrevScreen=SCREEN_HOME;
        currentScreen=SCREEN_MENU;
    }
    else if(btn == BTN_LEFT && currentScreen!=SCREEN_MENU && currentScreen!=SCREEN_TOOLS)
    {
        currentScreen=PrevScreen;
    }
    else if(btn == BTN_LEFT && currentScreen==SCREEN_MENU)
    {
        currentScreen=SCREEN_HOME;
        PrevScreen=SCREEN_HOME;
    }
    else if(btn == BTN_LEFT && currentScreen==SCREEN_TOOLS)
    {
        currentScreen=SCREEN_MENU;
        PrevScreen=SCREEN_HOME;
    }

    UI_render();
    _delay_ms(2);
}
}
