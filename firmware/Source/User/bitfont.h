#ifndef __BITFONT_H
#define __BITFONT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "hk32f030m.h" 

#define FONT_WIDTH 3
#define FONT_HEIGHT 4
#define FONT_MASK 0b111

extern uint8_t font[40][4];

#ifdef __cplusplus
}
#endif
#endif