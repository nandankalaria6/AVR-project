#ifndef UI_H
#define UI_H
typedef enum
{
    SCREEN_HOME,
    SCREEN_MENU,
    SCREEN_NOTIFICATIONS,
    SCREEN_SETTINGS,
    SCREEN_TEMP,
    SCREEN_BATTERY,
    SCREEN_DINO,
    SCREEN_TOOLS,
    SCREEN_STOPWATCH,
    SCREEN_DICE,
} Screen;

extern int velocityY;
extern uint8_t isJumping;
extern int dinoY ;
extern char currentTime[6];
extern char currentDate[9];
extern char temp[4];
extern char notification[32];
extern uint16_t notificationTimer;
extern uint8_t MusicState;
extern  uint8_t is_connected;
extern Screen currentScreen;
extern uint8_t selectedItem;
extern int cactusX;
extern uint8_t gameTick;
extern uint16_t score;
extern uint8_t gameSpeed;
extern uint8_t cactusType;
extern uint8_t gameOver;
extern uint8_t toolSelected;
extern uint32_t stopwatchMs;
extern uint8_t stopwatchRunning;
extern uint8_t diceValue;

#define TOOL_SIZE 2
#define MENU_SIZE 4
void UI_render();
#endif