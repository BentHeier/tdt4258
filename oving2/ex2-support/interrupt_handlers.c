#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "efm32gg.h"
#include "misc.h"

typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;

#define RAND_INIT { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }
pcg32_random_t globrand = RAND_INIT; 
uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}   

uint32_t rand(){
    return pcg32_random_r(&globrand);
}

double time = 0.0;
int sound_index = 0;

double frac(double t) {
	return t - (int) t;
}

uint32_t sound_0(double time) {
	double volume = 20.0, frq = 1000.0;
	double p = SAMPLE_PERIOD / frq;
	double val = volume * frac(time / p);
	return (uint32_t) val;
}

uint32_t sound_1(double time) {
	double volume = 50.0, frq = 2000.0;
	double p = SAMPLE_PERIOD / frq;
	double val = volume * frac(time / p);
	return (uint32_t) val;
}

uint32_t sound_2(double time) {
	double volume = 100.0, frq = 3000.0;
	double p = SAMPLE_PERIOD / frq;
	double val = volume * frac(time / p);
	return (uint32_t) val;
}

uint32_t sound_3(double time) {
	double volume = 200.0, frq = 4000.0;
	double p = SAMPLE_PERIOD / frq;
	double val = volume * frac(time / p);
	return (uint32_t) val;
}

void loop() {
	int buttons = 0, buttons_previous = 0;
	 
	while (1) {
		buttons_previous = buttons;
		buttons = *GPIO_PC_DIN;
		if ((buttons & 0x1) == 0){
			sound_index = 0;
		}
		if ((buttons & 0x2) == 0){
			sound_index = 1;
		}
		if ((buttons & 0x4) == 0){
			sound_index = 2;
		}
		if ((buttons & 0x8) == 0){
			sound_index = 3;
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
	
//	double val = volume * ((sin((time)/p) + 1.0f)/2.0f);
	uint32_t val;
	
	switch (sound_index) {
		case 0: val = sound_0(time); break;
		case 1: val = sound_1(time); break;
		case 2: val = sound_2(time); break;
		case 3: val = sound_3(time); break;
	}
	*DAC0_CH0DATA = (uint32_t)val;
 	*DAC0_CH1DATA = (uint32_t)val;
	time += 1.0;
	*TIMER1_IFC = 1;
}

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
