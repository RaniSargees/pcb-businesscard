#include "intro.h"
#include "games.h"
#include "drawing.h"
#include "hardware.h"
#include "bitfont.h"
#include "prng.h"
#include <hk32f030m.h>

#define MARQUEE_SPEED_ADJ 3
#define EMAIL_STR_LEN 22
#define BREAKOUT_STR_LEN 8
#define SNAKE_STR_LEN 5
#define PONG_STR_LEN 4

extern uint8_t buttons;
extern uint8_t button_change;

games intro() {
	
	int marquee_offset = 0;
	
	uint8_t text_name[EMAIL_STR_LEN] = {18, 1, 14, 9, 29, 19, 1, 18, 7, 5, 5, 19, 27, 7, 13, 1, 9, 12, 29, 3, 15, 13};
	
	//scrolling name/email display
	while (1) {
		update_buttons();
		clear_framebuffer();
		
		if (button_change & ~buttons & 0b000001) { //if a button released
			break;
		}
		
		draw_marquee(text_name, EMAIL_STR_LEN, 3, marquee_offset >> MARQUEE_SPEED_ADJ);
		
		marquee_offset = (marquee_offset + 1) % ((EMAIL_STR_LEN * (FONT_WIDTH + 1) + DISPLAY_WIDTH) << MARQUEE_SPEED_ADJ);
				
		update_screen();
	}
	
	marquee_offset = 0;
	
	games selected_game = game_snake;
	
	uint8_t text_breakout[BREAKOUT_STR_LEN] = {2, 18, 5, 1, 11, 15, 21, 20};
	uint8_t text_snake[SNAKE_STR_LEN] = {19, 14, 1, 11, 5};
	uint8_t text_pong[SNAKE_STR_LEN] = {16, 15, 14, 7};
	
	//game selection
	while (1) {
		update_buttons();
		clear_framebuffer();
		
		if (button_change & ~buttons & 0b000010) { //b button released
			return game_none;
		} else if (button_change & ~buttons & 0b000001) { //a button released
			seed(marquee_offset);
			return selected_game;
		} else if (button_change & ~buttons & 0b001000) { //left button released
			if (selected_game == 0) {selected_game = game_none;}
			selected_game = (selected_game - 1) % game_none;
			marquee_offset = 0;
		} else if (button_change & ~buttons & 0b000100) { //right button released
			selected_game = (selected_game + 1) % game_none;			
			marquee_offset = 0;
		}
		
		switch (selected_game) {
			case game_breakout:
				draw_marquee(text_breakout, BREAKOUT_STR_LEN, 3, marquee_offset >> MARQUEE_SPEED_ADJ);
				marquee_offset = (marquee_offset + 1) % ((BREAKOUT_STR_LEN * (FONT_WIDTH + 1) + DISPLAY_WIDTH) << MARQUEE_SPEED_ADJ);
				break;
			case game_snake:
				draw_marquee(text_snake, SNAKE_STR_LEN, 3, marquee_offset >> MARQUEE_SPEED_ADJ);
				marquee_offset = (marquee_offset + 1) % ((SNAKE_STR_LEN * (FONT_WIDTH + 1) + DISPLAY_WIDTH) << MARQUEE_SPEED_ADJ);
				break;
			case game_pong:
				draw_marquee(text_pong, PONG_STR_LEN, 3, marquee_offset >> MARQUEE_SPEED_ADJ);
				marquee_offset = (marquee_offset + 1) % ((PONG_STR_LEN * (FONT_WIDTH + 1) + DISPLAY_WIDTH) << MARQUEE_SPEED_ADJ);
				break;
			case game_none:
				selected_game = game_snake;
		}
		
		update_screen();
	}
}
