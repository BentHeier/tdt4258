#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "sounds.h"

// TIMER1 interrupt handler
void __attribute__ ((interrupt)) TIMER1_IRQHandler() {
	uint32_t val = sound_get_sample();
	*DAC0_CH0DATA = (uint32_t) val;
 	*DAC0_CH1DATA = (uint32_t) val;
	*TIMER1_IFC = 1;
}
