#ifndef __DRAWING_H
#define __DRAWING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define DISPLAY_WIDTH  10
#define DISPLAY_HEIGHT 11

typedef enum {
	down,
	right,
	up,
	left
} drawDir;

void clear_framebuffer();

void update_screen();

void draw_line(uint8_t x, uint8_t y, uint8_t len, drawDir dir);

void draw_dot(uint8_t x, uint8_t y);

void clear_dot(uint8_t x, uint8_t y);

void blit_line(uint8_t y, uint16_t mask, uint16_t line);

void blit_pos(uint8_t y, int8_t x, uint8_t len, uint8_t line);

void draw_marquee(uint8_t str[], uint8_t strlen, uint8_t y, int offset);

#ifdef __cplusplus
}
#endif
#endif