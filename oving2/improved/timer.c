#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

// function to setup the timer
void setupTimer(uint16_t period)
{
	// Enable timer1 clock 
	 *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;	
	// Set timer frequency
	 *TIMER1_TOP = period;
	// Enable timer interrupt generation
	 *TIMER1_IEN = 1;
	// Start timer
	 *TIMER1_CMD = 1;
}
