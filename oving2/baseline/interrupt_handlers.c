#include <stdint.h>
// TODO: Why tho?
#include <stdbool.h>

#include "efm32gg.h"
#include "sounds.h"

// TODO: Hvorfor bor denne her? Bør i ex2.c
void loop() {
	int buttons = 0;
	
	while (1) {
		buttons = *GPIO_PC_DIN;
		if ((buttons & 0x1) == 0){
			play_sound(1);
		}
		if ((buttons & 0x2) == 0){
			play_sound(2);
		}
		if ((buttons & 0x4) == 0){
			play_sound(3);
		}
	}
}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */
	//srand(14);
	uint32_t val = sound();	
//	double val = volume * ((sin((time)/p) + 1.0f)/2.0f);
	*DAC0_CH0DATA = (uint32_t)val;
 	*DAC0_CH1DATA = (uint32_t)val;
	*TIMER1_IFC = 1;
}

// TODO Bruke disse for knapp, ikke loop

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
}
