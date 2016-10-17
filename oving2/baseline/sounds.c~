#include "sounds.h"
#include "util.h"

#include <stdint.h>

int sound_index = 0;
double time = 0.0;

// Generates a sample for the startup sound. Returns the sample.
// time: A value which is 0.0 when the sound starts playing, and increases by 1.0 for each sample.
uint32_t sound_get_sample_startup(double time) {
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

// Generates a sample for the sound 1. Returns the sample.
// time: A value which is 0.0 when the sound starts playing, and increases by
//       1.0 for each sample.
uint32_t sound_get_sample_1(double time) {
	double volume = 30.0, frq = 50.0, length = 0.2 * SAMPLE_FREQUENCY;
	double p = SAMPLE_PERIOD / frq;
	double val;
	if (time > length)
		val = 0;
	else
		val = volume * frac(time / p);
	return (uint32_t) val;
}

// Generates a sample for the sound 2. Returns the sample.
// time: A value which is 0.0 when the sound starts playing, and increases by
//       1.0 for each sample.
uint32_t sound_get_sample_2(double time) {
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

// Generates a sample for the sound 3. Returns the sample.
// time: A value which is 0.0 when the sound starts playing, and increases by
//       1.0 for each sample.
uint32_t sound_get_sample_3(double time) {
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

// Stops the current sound from playing and plays a new sound.
// index: An index from 0 to 3 corresponding to the type of sound.
void sound_play(int index) {
	sound_index = index;
	time = 0.0;
}

// Returns the next sample of the sound that is currently playing.
uint32_t sound_get_sample() {
	uint32_t val;
	switch (sound_index) {
		case 0: val = sound_get_sample_startup(time); break;
		case 1: val = sound_get_sample_1(time); break;
		case 2: val = sound_get_sample_2(time); break;
		case 3: val = sound_get_sample_3(time); break;
	}
	time += 1.0;
	return val;
}
