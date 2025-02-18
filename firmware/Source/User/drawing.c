#include "drawing.h"
#include "hardware.h"
#include "bitfont.h"
#include <string.h>

uint16_t framebuffer[DISPLAY_HEIGHT] = {
	0b0000000000,
	0b0000000000,
	0b0000000000,
	0b0000000000,
	0b0000000000,
	0b0001001000,
	0b0001001000,
	0b0000000000,
	0b0010000100,
	0b0011111100,
	0b0000000000,
};

void clear_framebuffer() {
	memset(framebuffer, 0, sizeof framebuffer);
}

void update_screen() {
	//draw the framebuffer to the matrix
	for (uint8_t y = 0; y<11; ++y) {
		for (uint8_t x = 0; x<10; ++x) {
			control_matrix_led(9-x, y, (framebuffer[y]>>x) & 0b1);
			SOFT_DELAY_EXTREMELY_SHORT;
		}
	}
	
	control_matrix_led(0,0,0);
}

void draw_line(uint8_t x, uint8_t y, uint8_t len, drawDir dir) {
	switch (dir) {
		case up:
			y -= len - 1;
		case down:
			for (uint8_t z = y; z < y + len; ++z)
				framebuffer[z] |= 0b1<<(DISPLAY_WIDTH - 1 - x);
			break;
		
		case right:
			x += len - 1;
		case left:
			framebuffer[y] |= ((0b1<<len) - 1)<<(DISPLAY_WIDTH - 1 - x);
			break;
	}
}

void draw_dot(uint8_t x, uint8_t y) {
	framebuffer[y] |= 0b1<<(DISPLAY_WIDTH - 1 - x);
}

void clear_dot(uint8_t x, uint8_t y) {
	framebuffer[y] &= ~(0b1<<(DISPLAY_WIDTH - 1 - x));
}

void blit_line(uint8_t y, uint16_t mask, uint16_t line) {
	framebuffer[y] &= ~mask;
	framebuffer[y] |= line & mask;
}

void blit_pos(uint8_t y, int8_t x, uint8_t len, uint8_t line) {
	if ((x + len) < DISPLAY_WIDTH) {
		framebuffer[y] |= line << (DISPLAY_WIDTH - x - len);
	} else {
		framebuffer[y] |= line >> (len - DISPLAY_WIDTH + x);
	}
}

void draw_marquee(uint8_t str[], uint8_t strlen, uint8_t y, int marquee_offset) {
	for (uint8_t i=0; i<strlen; ++i) {
		for (uint8_t j=0; j<FONT_HEIGHT; ++j) {
			blit_pos(j + y, (FONT_WIDTH+1) * i + DISPLAY_WIDTH - marquee_offset, FONT_WIDTH + 1, font[str[i]][j]<<1);
		}
	}
}