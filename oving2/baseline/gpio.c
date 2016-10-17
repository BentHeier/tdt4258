#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

// Sets up GPIO mode and interrupts.
void setupGPIO() {
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; // Enables the GPIO clock.
	*GPIO_PA_CTRL = 2; // Sets high drive strength.
	*GPIO_PC_MODEL = 0x22222222; // Sets pins A0-7 as input.
	*GPIO_PA_MODEH = 0x55555555; // Sets pins A8-15 as output.
	*GPIO_PA_DOUT = 0xffffffff; // Turn off leds
	*GPIO_PC_DOUT = 0xff; // Enables internal pull-up.
}
