// Controls the game.

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "game.h"

float sqrt(float z){
    int val_int = *(int *)&z;
    val_int -= 1 << 23;
    val_int >>= 1;
    val_int += 1 << 29;

    return *(float *)&val_int;
}

int gameState = 0;
int game_finished = 0;
FILE *gamepad;
int timerStartDelay, timerEndDelay;
int leftBatX, leftBatY;
int rightBatX, rightBatY;
int leftBatYPrevious, rightBatYPrevious;
float ballX, ballY;
float ballXPrevious, ballYPrevious;
float ballVelocityX, ballVelocityY;
int turn = 0;
uint16_t *frameBuffer;

// Updates the game logic. Call this UPDATES_PER_SECOND times a second.
// uint8_t gamepadInput: Contains which gamepad buttons are pressed. Assumes the 8 least significant bits holds the data
//                        for the 8 buttons. The leftmost of these bits is for button 0, the next for button 1 etc. The bit
//				          should be set to 1 if the button is pressed and 0 otherwise.
void gameUpdate(uint8_t gamepadInput) {
    int i, x, y;
    float ballVelocityLength;
	switch (gameState) {
	
		// Game start state
		case 0:
			// If any of the buttons are pressed, change the state to the game running state.
			if ((gamepadInput & 0xFF) != 0) {
				timerStartDelay = UPDATES_PER_SECOND * 3; // Wait 3 seconds.
				
				// Sets the start positions for the two bats.
				leftBatX = GAME_WIDTH / 10;
				leftBatY = GAME_HEIGHT / 2;
				rightBatX = GAME_WIDTH - 1 - GAME_WIDTH / 10;
				rightBatY = GAME_HEIGHT / 2;
				
				// Sets the start position for the ball.
				ballX = GAME_WIDTH / 2;
				ballY = GAME_HEIGHT / 2;
				
				// Sets a velocity for the ball.
				switch (turn % 4) {
					case 0: ballVelocityX = 0.707 * BALL_SPEED; ballVelocityY = 0.707 * BALL_SPEED; break;
					case 1: ballVelocityX = -0.707 * BALL_SPEED; ballVelocityY = 0.707 * BALL_SPEED; break;
					case 2: ballVelocityX = 0.707 * BALL_SPEED; ballVelocityY = -0.707 * BALL_SPEED; break;
					case 3: ballVelocityX = -0.707 * BALL_SPEED; ballVelocityY = -0.707 * BALL_SPEED; break;
				}
				
				// Clears the screen to the background color.
				for (i = 0; i < GAME_WIDTH * GAME_HEIGHT; i += 1) {
					frameBuffer[i] = BACKGROUND_COLOR;
				}
				gameFrameBufferWasUpdated(0, 0, GAME_WIDTH, GAME_HEIGHT);
				
				// Proceeds to the next state, the running game state.
				gameState = 1;
			}
			break;
			
		// Game running state
		case 1:
			leftBatYPrevious = leftBatY;
			rightBatYPrevious = rightBatY;
			ballXPrevious = ballX;
			ballYPrevious = ballY;
		
			// If the startup timer is done, let the players control the bats and update the ball.
			if (timerStartDelay == 0) {
				// Lets the players control the bats with the upper and lower buttons on the gamepad.
				leftBatY += ((gamepadInput & BUTTON_LEFT_DOWN)
                        - (gamepadInput & BUTTON_LEFT_UP)) * BAT_SPEED;
				rightBatY += ((gamepadInput & BUTTON_RIGHT_DOWN)
                        - (gamepadInput & BUTTON_RIGHT_UP)) * BAT_SPEED;
				
				// Restricts the bats to be within the screen.
				if (leftBatY - BAT_HEIGHT / 2 < 0) {
					leftBatY = BAT_HEIGHT / 2;
				} else if (leftBatY + (BAT_HEIGHT / 2 - 1) > GAME_HEIGHT - 1) {
					leftBatY = (GAME_HEIGHT - 1) - (BAT_HEIGHT / 2 - 1);
				}
				if (rightBatY - BAT_HEIGHT / 2 < 0) {
					rightBatY = BAT_HEIGHT / 2;
				} else if (rightBatY + (BAT_HEIGHT / 2 - 1) > GAME_HEIGHT - 1) {
					rightBatY = (GAME_HEIGHT - 1) - (BAT_HEIGHT / 2 - 1);
				}
				
				// Updates the ball position.
				ballX += ballVelocityX;
				ballY += ballVelocityY;
				
				// If the game is over, go to the game end state, otherwise bounce the ball if it hit something.
				if (ballX < 0 || ballX >= GAME_WIDTH) {	
					timerEndDelay = UPDATES_PER_SECOND * 2; // Wait 2 seconds before restarting the game.
					gameState = 2;
				} else {
					// Lets the ball bounce off the top and bottom boundaries of the screen.
					if (ballY < 0) {
						ballY = -ballY;
						ballVelocityY = -ballVelocityY;
					} else if (ballY >= GAME_HEIGHT) {
						ballY = (GAME_HEIGHT - 1) - (ballY - (GAME_HEIGHT - 1));
						ballVelocityY = -ballVelocityY;
					}
					
					// Lets the ball bounce off bats.
					if (ballX <= leftBatX && ballX - ballVelocityX > leftBatX && ballY >= leftBatY - BAT_HEIGHT / 2 && ballY <= leftBatY + (BAT_HEIGHT / 2 - 1)) {
						ballX = leftBatX + 1;
						ballVelocityX = BAT_HEIGHT / 2;
						ballVelocityY = ballY - leftBatY;
						ballVelocityLength = sqrt(ballVelocityX * ballVelocityX + ballVelocityY * ballVelocityY);
						ballVelocityX /= ballVelocityLength; ballVelocityY /= ballVelocityLength;
						ballVelocityX *= BALL_SPEED; ballVelocityY *= BALL_SPEED;
					} else if (ballX >= rightBatX && ballX - ballVelocityX < rightBatX && ballY >= rightBatY - BAT_HEIGHT / 2 && ballY <= rightBatY + (BAT_HEIGHT / 2 - 1)) {
						ballX = rightBatX - 1;
						ballVelocityX = BAT_HEIGHT / 2;
						ballVelocityY = ballX - rightBatX;
						ballVelocityLength = sqrt(ballVelocityX * ballVelocityX + ballVelocityY * ballVelocityY);
						ballVelocityX /= ballVelocityLength; ballVelocityY /= ballVelocityLength;
						ballVelocityX *= BALL_SPEED; ballVelocityY *= BALL_SPEED;
					}
				}
			} else {
				timerStartDelay -= 1;
			}
			
			// Updates the areas in the frame buffer where the bats and ball are by setting the old areas to the background color,
			// and the new ones to the bat and ball colors.
			for (y = leftBatYPrevious - BAT_GRAPHICS_HEIGHT / 2; y < leftBatYPrevious + BAT_GRAPHICS_HEIGHT / 2 - 1; y += 1) {
				for (x = leftBatX- BAT_GRAPHICS_WIDTH / 2; x < leftBatX+ BAT_GRAPHICS_WIDTH / 2 - 1; x += 1) {
					frameBuffer[y * GAME_WIDTH + x] = BACKGROUND_COLOR;
				}
			}
			gameFrameBufferWasUpdated(leftBatX, leftBatYPrevious, BAT_GRAPHICS_WIDTH, BAT_GRAPHICS_HEIGHT);
			for (y = rightBatYPrevious - BAT_GRAPHICS_HEIGHT / 2; y < rightBatYPrevious + BAT_GRAPHICS_HEIGHT / 2 - 1; y += 1) {
				for (x = rightBatX - BAT_GRAPHICS_WIDTH / 2; x < rightBatX + BAT_GRAPHICS_WIDTH / 2 - 1; x += 1) {
					frameBuffer[y * GAME_WIDTH + x] = BACKGROUND_COLOR;
				}
			}
			gameFrameBufferWasUpdated(rightBatX, rightBatYPrevious, BAT_GRAPHICS_WIDTH, BAT_GRAPHICS_HEIGHT);
			for (y = ballYPrevious - BALL_GRAPHICS_HEIGHT / 2; y < ballYPrevious + BALL_GRAPHICS_HEIGHT / 2 - 1; y += 1) {
				for (x = ballXPrevious - BALL_GRAPHICS_WIDTH / 2; x < ballXPrevious + BALL_GRAPHICS_WIDTH / 2 - 1; x += 1) {
					frameBuffer[y * GAME_WIDTH + x] = BACKGROUND_COLOR;
				}
			}
			gameFrameBufferWasUpdated(ballXPrevious, ballYPrevious, BALL_GRAPHICS_WIDTH, BALL_GRAPHICS_HEIGHT);
			for (y = leftBatY - BAT_GRAPHICS_HEIGHT / 2; y < leftBatY + BAT_GRAPHICS_HEIGHT / 2 - 1; y += 1) {
				for (x = leftBatX - BAT_GRAPHICS_WIDTH / 2; x < leftBatX + BAT_GRAPHICS_WIDTH / 2 - 1; x += 1) {
					frameBuffer[y * GAME_WIDTH + x] = LEFT_BAT_COLOR;
				}
			}
			gameFrameBufferWasUpdated(leftBatX, leftBatY, BAT_GRAPHICS_WIDTH, BAT_GRAPHICS_HEIGHT);
			for (y = rightBatY - BAT_GRAPHICS_HEIGHT / 2; y < rightBatY + BAT_GRAPHICS_HEIGHT / 2 - 1; y += 1) {
				for (x = rightBatX - BAT_GRAPHICS_WIDTH / 2; x < rightBatX + BAT_GRAPHICS_WIDTH / 2 - 1; x += 1) {
					frameBuffer[y * GAME_WIDTH + x] = RIGHT_BAT_COLOR;
				}
			}
			gameFrameBufferWasUpdated(rightBatX, rightBatY, BAT_GRAPHICS_WIDTH, BAT_GRAPHICS_HEIGHT);
			for (y = ballY - BALL_GRAPHICS_HEIGHT / 2; y < ballY + BALL_GRAPHICS_HEIGHT / 2 - 1; y += 1) {
				for (x = ballX - BALL_GRAPHICS_WIDTH / 2; x < ballX + BALL_GRAPHICS_WIDTH / 2 - 1; x += 1) {
					frameBuffer[y * GAME_WIDTH + x] = BALL_COLOR;
				}
			}
			gameFrameBufferWasUpdated(ballX, ballY, BALL_GRAPHICS_WIDTH, BALL_GRAPHICS_HEIGHT);
			break;
			
		// Game end state
		case 2:
			// If the end timer is done, go to the next turn.
			if (timerEndDelay == 0) {
				turn += 1;
				gameState = 0;
			} else {
				timerEndDelay -= 1;
			}
			break;
	}
}

int fb_handle;
struct fb_copyarea update_region;

// This function is called whenever a region within the frame buffer is updated.
// x: The x position for where the frame was updated.
// y: The y position for where the frame was updated.
// width: The width of the area updated.
// height: The height of the area updated.
void gameFrameBufferWasUpdated(int x, int y, int width, int height) {
    update_region.dx = x;
    update_region.dy = y;
    update_region.width = width;
    update_region.height = height;
    ioctl(fb_handle, 0x4680, &update_region);
}

// Returns a pointer to where the frame begins. The frame is composed of GAME_WIDTH * GAME_HEIGHT uint16_ts.
// Every uint16_t contains 5 bits of red, 6 bits of green, and 5 bits of blue (in that order with red being most significant).
uint16_t *gameGetFrameBuffer() {	
	return frameBuffer;
}


int init_gfx(int width, int height, uint16_t *buffer){

    fb_handle = open("/dev/fb0", O_RDWR);
    if(fb_handle < 0){
        printf("Failed to open /dev/fb0");
        return -1;
    }

    frameBuffer = (uint16_t *)mmap(NULL, width*height*2,
            PROT_READ | PROT_WRITE, MAP_SHARED, fb_handle, 0);
    return 0;
}

void cleanup(){
    munmap(frameBuffer, GAME_WIDTH*GAME_HEIGHT*2);
    close(fb_handle);
    fclose(gamepad);
}

void printBtns(uint8_t state){
    printf("state: %d\n", state);
}

int main(int argc, char **argv){
    uint8_t btn_state;
    struct timespec sleepValue = {0};
    sleepValue.tv_nsec = 1000000000/UPDATES_PER_SECOND;

    init_gfx(GAME_WIDTH, GAME_HEIGHT, frameBuffer);
    
    gamepad = fopen("/dev/gamepad", "rb");

    while(!game_finished){
        btn_state = fgetc(gamepad);
        gameUpdate(btn_state);
        nanosleep(&sleepValue, NULL);
    }

    cleanup();
    return 0;
}
