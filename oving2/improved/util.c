#include <stdint.h>

typedef struct {uint64_t state;  uint64_t inc;} pcg32_random_t;

#define RAND_INIT {0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL}

pcg32_random_t globrand = RAND_INIT;

// Generates and returns a random number from 0 to 2^32-1 from an rng.
// rng: The rng to use.
uint32_t pcg32_random_r(pcg32_random_t* rng) {
    uint64_t oldstate = rng->state;
    // Advances internal state.
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculates output function (XSH RR). Uses old state for max ILP.
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}   

// Returns a random number from 0 to 2^32-1.
uint32_t rand() {
    return pcg32_random_r(&globrand);
}

// Returns the fractional part of a number.
// t: The number.
double frac(double t) {
	return t - (int) t;
}
