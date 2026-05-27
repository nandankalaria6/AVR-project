#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "ui.h"
#include "graphics.h"
#include "oled.h"

int dinoY = 42;
int velocityY = 0;
uint8_t isJumping = 0;
uint8_t gameTick = 0;
uint16_t score = 0;
char currentTime[6] = "00:00";
char currentDate[9] = "00/00/00";
char temp[4]="55.5";
char notification[32] = "No Notification";
typedef struct
{
    Screen screen;
    uint8_t selectedItem;
} UIState;

UIState ui;
uint16_t notificationTimer = 0;
Screen currentScreen = SCREEN_HOME;
uint8_t selectedItem = 0;
uint8_t MusicState=0;
int cactusX = 120;
uint8_t cactusType = 0;
uint8_t gameSpeed = 3;
uint8_t gameOver = 0;
uint8_t groundOffset=128;

extern uint32_t stopwatchMs=1;
extern uint8_t stopwatchRunning=1;
uint8_t diceValue = 1;
const char* menuItems[] =
{
    "Tools",
    "temp",
    "Settings",
    "Dino"
};
const char* toolItems[] =
{
    "Stopwatch",
    "Dice",
};
uint8_t toolSelected = 0;
#define TOOL_SIZE 2
#define MENU_SIZE 4
void drawMenu()
{
    for(uint8_t i=0;i<MENU_SIZE;i++)
    {
        if(i == selectedItem)
        {
            drawRect(0, i*10, 128, 9);
        }
        drawString(10, i*10, (char*)menuItems[i]);
    }
}
    uint8_t is_connected;
void drawHome()
{
    drawString(30,0,"DEVICE");
    drawRect(0,28,128,9);
    if (PIND & (1 << PD6))is_connected = 1; 
    else is_connected = 0;

    if (is_connected) {
        drawString(10, 30, "CONNECTED");
    } else {
        drawString(10, 30, "DISCONNECTED");
    }
    drawString(20,10,currentTime);
    drawString(10,20,currentDate);
    
}
void drawDino()
{
            drawPixel(groundOffset,51,1);
            drawPixel((groundOffset+40)%128,52,1);
            drawPixel((groundOffset+80)%128,51,1);
            drawHLine(4,49,118);
            if(cactusType == 0)
            {
                drawRect(cactusX,42,4,8);
                drawRect(cactusX+4,46,2,4);
            }
            else if(cactusType == 1)
            {
                drawRect(cactusX,38,4,12);
                drawRect(cactusX+4,42,2,4);
            }
            else
            {
                drawRect(cactusX,44,8,6);
            }
            //drawRect(10,dinoY,8,8);
            if(isJumping)
            {
                drawRect(10,dinoY+2,8,6);
                drawPixel(18,dinoY+3,1);
            }
            else 
            {
                drawRect(10,dinoY,8,8);
                drawPixel(18,dinoY+2,1);
            }
            if(cactusX < 18 && cactusX > 4)
            {
                if(dinoY > 34)
                {
                    gameOver = 1;
                }
            }
            if(!gameOver)
            {
                gameTick++;

                if(gameTick >= 1)
                {
                    gameTick = 0;
                    dinoY += velocityY;
                    velocityY+=(2+score/10);
                    if(dinoY >= 42)
                    {
                        dinoY = 42;
                        velocityY = 0;
                        isJumping = 0;
                    }
                    cactusX -= gameSpeed;
                    groundOffset-=gameSpeed;

                    if(cactusX < -6)
                    {
                        cactusType++;
                        if(cactusType > 2)
                        {
                            cactusType = 0;
                        }
                        cactusX=128;
                        score++;
                        if(score % 5 == 0)
                        {
                            gameSpeed++;
                        }
                    }
                }
            }
            if(gameOver)
            {
            drawString(30,20,"GAME OVER");
            drawString(20,30,"PRESS RIGHT");
            }
            drawString(30,0,"DINO");
            char scoreStr[16];
            sprintf(scoreStr,"%u",score);

            drawString(90,0,scoreStr);


}
void drawTools()
{
    for(uint8_t i=0;i<3;i++)
    {
        if(i == toolSelected)
        {
            drawRect(0,i*10,128,9);
        }

        drawString(10,i*10,(char*)toolItems[i]);
    }
}

void drawStopwatch()
{
    uint16_t seconds = stopwatchMs / 1000;
    uint16_t minutes = seconds / 60;
    seconds %= 60;
    uint16_t centiseconds = (stopwatchMs % 1000) / 10;
    char timeStr[20];
    sprintf(timeStr,"%02u:%02u:%02u",
            minutes,
            seconds,
            centiseconds);
    drawString(20,20,timeStr);
}
void drawDice()
{
    char diceStr[10];
    char diceStr20[10];
    char diceStr12[10];

    sprintf(diceStr,"D6 = %u",diceValue%7);
    sprintf(diceStr12,"D12 = %u",diceValue%13);
    sprintf(diceStr20,"D20 = %u",diceValue);

    drawString(30,10,diceStr);
    drawString(30,29,diceStr12);
    drawString(30,48,diceStr20);
}

void UI_render()
{
    clearBuffer();
    switch(currentScreen)
    {
        case SCREEN_HOME:
            drawHome();
            break;

        case SCREEN_MENU:
            drawMenu();
            break;
        case SCREEN_NOTIFICATIONS:
            drawString(0,0,notification);
            break;
        case SCREEN_SETTINGS:
            drawString(0,0,"SETTINGS");
            break;
        case SCREEN_TEMP
        :
            drawString(0,0,"Temperature :");
            drawString(70,0,temp);
            break;
        case SCREEN_BATTERY:
            drawString(10,10,"Battery = 67%%");
            break;
        case SCREEN_DINO:
            drawDino();
            break;
        case SCREEN_TOOLS:
            drawTools();
            break;
        case SCREEN_STOPWATCH:
            drawStopwatch();
            break;
        case SCREEN_DICE:
            drawDice();
    }
    OLED_update();
}
