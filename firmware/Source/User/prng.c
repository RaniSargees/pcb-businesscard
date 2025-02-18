#include "prng.h"
#include "hk32f030m.h" 

uint64_t state;

void seed(uint64_t x) {
	state = x;
	if (state == 0) {
		++state;
	}
}

uint64_t random() {
  state ^= (state << 21);
  state ^= (state >> 35);
  state ^= (state << 4);
  return state - 1;
}