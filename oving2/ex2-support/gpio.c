#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void setupGPIO()
{
	/* Example of HW access from C code: turn on joystick LEDs D4-D8
	   check efm32gg.h for other useful register definitions
	 */
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	/* enable GPIO clock */
	*GPIO_PA_CTRL = 2;	/* set high drive strength */
	*GPIO_PC_MODEL = 0x22222222;	/* set pins A0-7 as input */
	*GPIO_PA_MODEH = 0x55555555;	/* set pins A8-15 as output */
	*GPIO_PA_DOUT = 0xaa00;	/* turn on LEDs D4-D8 (LEDs are active low) */
	*GPIO_PC_DOUT = 0xFF;	/* enable internal pull-up */
}
