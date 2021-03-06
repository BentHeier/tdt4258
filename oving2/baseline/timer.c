#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

// Enables and sets up the timer.
// period: The number of cycles between each timer interrupt.
void setupTimer(uint16_t period) {
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1; // Enables timer1 clock.
	*TIMER1_TOP = period; // Sets the timer1 period.
	*TIMER1_IEN = 1; // Enable interrupts
	*TIMER1_CMD = 1; // Start timer
}
