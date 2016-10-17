#include <stdint.h>
// TODO: Why tho?
#include <stdbool.h>

#include "efm32gg.h"
#include "sounds.h"

#define TIMEOUT 10000

void handleButtons() {
	int buttons = 0;

	*GPIO_PA_DOUT = (1 << 8);

	buttons = *GPIO_PC_DIN;
	if ((buttons & 0x1) == 0){
		*SCR = 2;
		play_sound(1);
		*TIMER1_CMD = 1;
	}
	if ((buttons & 0x2) == 0){
		*SCR = 2;
		play_sound(2);
		*TIMER1_CMD = 1;
	}
	if ((buttons & 0x4) == 0){
		*SCR = 2;
		play_sound(3);
		*TIMER1_CMD = 1;
	}
}

uint32_t prev_val = 0;
unsigned int counter = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	*GPIO_PA_DOUT = (1 << 9);
	uint32_t val = sound();	
	*DAC0_CH0DATA = (uint32_t)val;
 	*DAC0_CH1DATA = (uint32_t)val;
	if(val == prev_val){
		counter+=1;
	} else {
		counter=0;
	}
	if(counter > TIMEOUT){
		counter = 0;
		*GPIO_PA_DOUT = (1 << 10);
		*TIMER1_CMD = 2;
		*SCR = 6;
	}
	prev_val = val;
	*TIMER1_IFC = 1;
}

// TODO Bruke disse for knapp, ikke loop

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	handleButtons();
	*GPIO_IFC = 0xff;
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	handleButtons();
	*GPIO_IFC = 0xff;
}
