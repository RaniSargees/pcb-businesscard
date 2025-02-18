#ifndef __PRNG_H
#define __PRNG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hk32f030m.h" 

void seed(uint64_t x);

uint64_t random();

#ifdef __cplusplus
}
#endif
#endif