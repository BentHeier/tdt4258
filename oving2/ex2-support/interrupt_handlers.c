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
	double volume = 30.0, frq, length = 5.0 * SAMPLE_FREQUENCY;
	double p;
	double val;
	if (time > length)
		val = 0;
	else {
		if (time < length * 0.1)
			frq = 200.0;
		else if (time < length * 0.2)
			frq = 300.0;
		else if (time < length * 0.3)
			frq = 400.0;
		else if (time < length * 0.4)
			frq = 800.0;
		else if (time < length * 0.5)
			frq = 500.0;
		else if (time < length * 0.6)
			frq = 200.0;
		else if (time < length * 0.7)
			frq = 300.0;
		else if (time < length * 0.8)
			frq = 500.0;
		else if (time < length * 0.9)
			frq = 600.0;
		else
			frq = 300.0;
		p = SAMPLE_PERIOD / frq;
		val = volume * frac(time / p);
	}
	return (uint32_t) val;
}

uint32_t sound_1(double time) {
	double volume = 30.0, frq = 50.0, length = 0.2 * SAMPLE_FREQUENCY;
	double p = SAMPLE_PERIOD / frq;
	double val;
	if (time > length)
		val = 0;
	else
		val = volume * frac(time / p);
	return (uint32_t) val;
}

uint32_t sound_2(double time) {
	double volume = 30.0, frq, length = 4.0 * SAMPLE_FREQUENCY;
	double p;
	double val;
	if (time > length)
		val = 0;
	else {
		frq = ((length - time * 2.0) / SAMPLE_FREQUENCY) * 1000.0;
		p = SAMPLE_PERIOD / frq;
		val = volume * (frac(time / p) + 0.001 * (rand() % 100));
	}
	return (uint32_t) val;
}

uint32_t sound_3(double time) {
	double volume = 30.0, frq, length = 2.0 * SAMPLE_FREQUENCY;
	double p;
	double val;
	if (time > length) {
		val = 0;
	} else {
		frq = 200.0 + ((length - time) / SAMPLE_FREQUENCY) * 100.0;
		p = SAMPLE_PERIOD / frq;
		val = volume * (frac(time / p));
	}
	return (uint32_t) val;
}

void play_sound(int index) {
	sound_index = index;
	time = 0.0;
}

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
