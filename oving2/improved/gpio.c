#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

// Function to set up GPIO mode and interrupts
void setupGPIO() {
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; // Enable GPIO clock
	*GPIO_PA_CTRL = 2; // Set high drive strength
	*GPIO_PC_MODEL = 0x22222222; // Set pins A0-7 as input
	*GPIO_PA_MODEH = 0x55555555; // Set pins A8-15 as output 
	*GPIO_PA_DOUT = 0xffffffff; // Turn off LEDs
	*GPIO_PC_DOUT = 0xFF; // Enable internal pull-up 

	// Set up interrupt handling
	*GPIO_EXTIPSELL = 0x22222222;
	// ... for falling edge
	*GPIO_EXTIFALL = 0xff;
	*GPIO_IEN = 0xff;
}
