#include <stdint.h>

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

// TODO: Kan bo i egen fil (kanskje sammen med random, utils.h eller noe)
double frac(double t) {
	return t - (int) t;
}


