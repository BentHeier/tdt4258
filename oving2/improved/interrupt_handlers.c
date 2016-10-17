#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sounds.h"

// The amount of samples of silence required before stopping timer.
#define TIMEOUT 10000

// Start playing a sound by index i.
void start_sound(int i) {
	// Disable deep sleep (This is necessary for timer to work).
	*SCR = 2;
	// Select sound
	sound_play(i);
	// Start timer
	*TIMER1_CMD = 1;
}

// Handle button presses
void handleButtons() {
	// Read current button values.
	int buttons = 0;
	buttons = *GPIO_PC_DIN;
	// Check for button 1, 2 or 3, and start respective sound.
	if ((buttons & 0x1) == 0){
		start_sound(1);
	}
	if ((buttons & 0x2) == 0){
		start_sound(2);
	}
	if ((buttons & 0x4) == 0){
		start_sound(3);
	}
}

// Initiate variables for end-of-sound detection.
uint32_t prev_val = 0;
unsigned int counter = 0;

// TIMER1 interrupt handler 
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	// Get next sample and write it to DAC:
	uint32_t val = sound_get_sample();	
	*DAC0_CH0DATA = (uint32_t)val;
 	*DAC0_CH1DATA = (uint32_t)val;
	// Check if we have silence for more than TIMEOUT samples
	if(val == prev_val){
		counter+=1;
	} else {
		counter=0;
	}
	if(counter > TIMEOUT){
		// If we have long silence, disable timer and enter deep sleep.
		counter = 0;
		*TIMER1_CMD = 2;
		*SCR = 6;
	}
	prev_val = val;
	*TIMER1_IFC = 1;
}

// GPIO even pin interrupt handler 
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	handleButtons();
	*GPIO_IFC = 0xff;
}

// GPIO odd pin interrupt handler 
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	handleButtons();
	*GPIO_IFC = 0xff;
}
