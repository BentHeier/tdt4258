#include <stdint.h>

#define UPDATES_PER_SECOND 30
#define GAME_WIDTH 320
#define GAME_HEIGHT 240
#define BAT_SPEED 3
#define BAT_HEIGHT 32 // Should be an even number.
#define BALL_SPEED 3

#define BACKGROUND_COLOR 0x0000
#define LEFT_BAT_COLOR 0xFFFF
#define RIGHT_BAT_COLOR 0xFFFF
#define BALL_COLOR 0xFFFF
#define BALL_GRAPHICS_WIDTH 4 // Should be an even number.
#define BALL_GRAPHICS_HEIGHT 4 // Should be an even number.
#define BAT_GRAPHICS_WIDTH 4 // Should be an even number.
#define BAT_GRAPHICS_HEIGHT BAT_HEIGHT

#define BUTTON_LEFT_UP = (1 << 1)
#define BUTTON_LEFT_DOWN = (1 << 3)
#define BUTTON_RIGHT_UP = (1 << 5)
#define BUTTON_RIGHT_DOWN = (1 << 7)
