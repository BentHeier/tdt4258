#ifndef SOUNDS_H
#define SOUNDS_H

#include <stdint.h>

#define SAMPLE_FREQUENCY 4410
#define SAMPLE_PERIOD 14000000 / SAMPLE_FREQUENCY

void sound_play(int index);
uint32_t sound_get_sample();

#endif
