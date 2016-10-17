#include "sounds.h"

#include <stdint.h>

#include "util.h"

int sound_index = 0;
double time = 0.0;

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

uint32_t sound(){
	uint32_t val;
	switch (sound_index) {
		case 0: val = sound_0(time); break;
		case 1: val = sound_1(time); break;
		case 2: val = sound_2(time); break;
		case 3: val = sound_3(time); break;
	}
	time += 1;
	return val;
}
