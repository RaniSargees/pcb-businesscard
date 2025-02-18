#include "drawing.h"
#include "hardware.h"
#include "prng.h"
#include "snake.h"
#include <hk32f030m.h>

extern uint8_t buttons;
extern uint8_t button_change;

void snake() {
	
	typedef enum {
		up,
		down,
		left,
		right
	} directions;
	
	uint16_t grid[DISPLAY_HEIGHT][DISPLAY_WIDTH] = {{0}};

	uint8_t needs_fruit = 1;
	uint8_t direction = right;
	uint8_t x = 0;
	uint8_t y = DISPLAY_HEIGHT / 2;
	uint8_t to_move = 0;
	uint8_t to_grow = 0;
	uint8_t length = 1;
	uint8_t endgame = 0;
	
	grid[y][x] = 1;
	
	while (1) {
		update_buttons();
		clear_framebuffer();
		
		switch (button_change & buttons & 0b111100) {
			case 0b100000:
				direction = up;
				break;
			case 0b010000:
				direction = down;
				break;
			case 0b001000:
				direction = left;
				break;
			case 0b000100:
				direction = right;
				break;
		}
		
		to_move = (to_move + 1) % 12;
		
		if (needs_fruit) {
			needs_fruit = 0;
			uint8_t new_fruit_x;
			uint8_t new_fruit_y;
			while(grid[(new_fruit_y = (random()%DISPLAY_HEIGHT))][(new_fruit_x = (random()%DISPLAY_WIDTH))] != 0);
			grid[new_fruit_y][new_fruit_x] = UINT16_MAX;
		}
		
		if (!to_move) {
			switch(direction) {
				case up:
					--y;
					break;
				case down:
					++y;
					break;
				case left:
					--x;
					break;
				case right:
					++x;
					break;
			}
			
			if (y >= DISPLAY_HEIGHT || x >= DISPLAY_WIDTH || (length + 1) == DISPLAY_HEIGHT * DISPLAY_WIDTH) { //relies on overflow lol
				endgame = 1;
			} else {
				if (to_grow) {
					++length;
					to_grow = 0;
				}
				if (grid[y][x] == UINT16_MAX) {
					to_grow = 1;
					needs_fruit = 1;
				} else if (grid[y][x]>1) {
					endgame = 1;
				}
				grid[y][x] = length + 1;
			}
		}
		
		if (endgame) {
			break;
		}
		
		for (uint8_t i=0; i<DISPLAY_HEIGHT; ++i) {
			for (uint8_t j=0; j<DISPLAY_WIDTH; ++j) {
				if (grid[i][j])	{
					draw_dot(j,i);
					if (grid[i][j] != UINT16_MAX && !to_grow && !to_move) {
						--grid[i][j];
					}
				}
			}
		}
		update_screen();
	}
	
	endgame = 100;
	
	while(endgame--) {
		clear_framebuffer();
		for (uint8_t i=0; i<DISPLAY_HEIGHT; ++i) {
			for (uint8_t j=0; j<DISPLAY_WIDTH; ++j) {
				if (!grid[i][j])	{
					draw_dot(j,i);
				}
			}
		}
		update_screen();
	}
}