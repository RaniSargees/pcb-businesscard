#include "hardware.h"
#include "games.h"
#include "intro.h"
#include "snake.h"

#include "drawing.h"
#include <hk32f030m.h>

extern uint8_t buttons;
extern uint8_t button_change;

#define PADDLE_WIDTH 3
#define PADDLE_SPEED_ADJ 4

#define BALL_SPEED_ADJ 3

extern uint16_t framebuffer[DISPLAY_HEIGHT];

void breakout() {
	uint8_t ball_x = (DISPLAY_WIDTH << BALL_SPEED_ADJ) / 2;
	uint8_t ball_y = (DISPLAY_HEIGHT - 2) << BALL_SPEED_ADJ;
	int8_t ball_vspeed = -1;
	int8_t ball_hspeed = 1;
	uint8_t paddle_pos = (DISPLAY_WIDTH << PADDLE_SPEED_ADJ) / 2 - (PADDLE_WIDTH << PADDLE_SPEED_ADJ) / 2;
	uint8_t temp1;
	uint8_t temp2;
	
	uint8_t bricks[DISPLAY_HEIGHT / 2 - 1][DISPLAY_WIDTH / 2] = {{0}};
	
	while (1) {
		update_buttons();
		
		//move paddle
		if (buttons & 0b001000) { //left
			if (paddle_pos > 0) {
				--paddle_pos;
			}
		} else if (buttons & 0b000100) { //right
			if ((paddle_pos >> PADDLE_SPEED_ADJ) < (DISPLAY_WIDTH - PADDLE_WIDTH)) {
				++paddle_pos;
			}
		}

		//bounce off sides
		if (((ball_x + ball_hspeed) >> BALL_SPEED_ADJ) >= DISPLAY_WIDTH) {
			ball_hspeed *= -1;
		}

		//bounce off ceiling
		if (ball_y == 0 && ball_vspeed < 0) {
			ball_vspeed *= -1;
		}

		//bounce off the paddle, angle dependent on paddle position
		//this is super complicated because the ball and paddle actually work in
		//different coordinate spaces; the paddle having double the range of the
		//ball. That's how the speed adjustment is handled. it works ¯\_(''/)_/¯
		if (((ball_y + ball_vspeed) >> BALL_SPEED_ADJ) == (DISPLAY_HEIGHT - 1)
		&& (ball_x + ball_hspeed) >= (paddle_pos >> (PADDLE_SPEED_ADJ - BALL_SPEED_ADJ))
		&& (ball_x + ball_hspeed) <= ((paddle_pos + (PADDLE_WIDTH << PADDLE_SPEED_ADJ))
		>> (PADDLE_SPEED_ADJ - BALL_SPEED_ADJ))) {
			ball_vspeed *= -1;
			ball_hspeed = (5 *((ball_x - (paddle_pos >> (PADDLE_SPEED_ADJ - BALL_SPEED_ADJ)))) / (PADDLE_WIDTH << BALL_SPEED_ADJ)) - 2;
		}

		//check for bricks
		temp1 = (ball_y + ball_vspeed) >> BALL_SPEED_ADJ;
		if (temp1 <= (DISPLAY_HEIGHT / 2 - 1)
		&& !bricks[temp1][(temp2=((ball_x + ball_hspeed) >> (BALL_SPEED_ADJ + 1)))]) {
			bricks[temp1][temp2] = 1;
		}
		
		//move ball
		ball_x += ball_hspeed;
		ball_y += ball_vspeed;


		if ((ball_y >> BALL_SPEED_ADJ) >= DISPLAY_HEIGHT) {
			break; //end the game
		}

		//drawing
		clear_framebuffer();

		draw_dot(ball_x >> BALL_SPEED_ADJ, ball_y >> BALL_SPEED_ADJ);

		draw_line(paddle_pos >> PADDLE_SPEED_ADJ, DISPLAY_HEIGHT - 1, PADDLE_WIDTH, right);

		for (uint8_t i = 0; i<DISPLAY_HEIGHT / 2 - 1; ++i) {
			for (uint8_t j = 0; j<DISPLAY_WIDTH / 2; ++j) {
				if (!bricks[i][j]) {
					draw_dot(j << 1, i);
					draw_dot((j << 1) + 1, i);
				}
			}
		}
		update_screen();
	}

	//reuse this variable for delay
	paddle_pos = 200;

	//invert the framebuffer
	for(uint8_t i = 0; i<DISPLAY_HEIGHT; ++i) {
		blit_line(i, 0xffff, ~framebuffer[i]);
	}

	//display end screen
	while(paddle_pos--) {
		update_screen();
	}
}

void pong() {

}

int main(void) {
	//bare minimum to be able to wake up from sleep on a button press
	init_gpio_clocks();
	init_button_gpios();
	
	//this is the "power off" mode, since there is no power switch
	sleep();
	
	//the rest (LED GPIOs and ADC)
	init_matrix_gpios();
	init_dpad_gpio();
	
	while (1) {
		switch(intro()) {
			case game_snake:
				snake();
				break;
			case game_breakout:
				breakout();
				break;
			case game_pong:
				pong();
				break;
			case game_none:
				break;
		}
	}
}


