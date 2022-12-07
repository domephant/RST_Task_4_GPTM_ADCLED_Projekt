#include "game.h"

uint8_t pos_x, pos_y;
uint8_t tar_x, tar_y;
uint16_t score;
uint16_t timer;

float rad;

float absf(float val){
	if(val < 0){
		return -val;
	}else{
		return val;
	}
}

void game_init(){
	pos_x = 64;
	pos_y = 64;
	
	tar_x = 64;
	tar_y = 64;
	
	score = 0;
	
	lcd_fill(COLOR_BLACK);
	
	timer = 250;
	
	rad = 15;
	
	lcd_circle_fill(tar_x, tar_y, 5, COLOR_RED);
}

void game_update(uint8_t x, uint8_t y){
	lcd_circle_fill(pos_x, pos_y, 2, COLOR_BLACK);
	
	pos_x = x;
	pos_y = y;
	
	rand();
		
	if(absf(tar_x - pos_x) < rad + 2 && absf(tar_y - pos_y) < rad + 2){
		lcd_circle_fill(tar_x, tar_y, rad, COLOR_BLACK);
		tar_x = ((float)rand())/RAND_MAX * 64 + 32;
		tar_y = ((float)rand())/RAND_MAX * 64 + 32;
		
		rad -= 0.25f;
		if(rad < 1)
			rad = 1;
		
		lcd_circle_fill(tar_x, tar_y, rad, COLOR_RED);
				
		score++;
		timer = 150;
	}
	
		
	lcd_circle_fill(pos_x, pos_y, 2, COLOR_GREEN);
	
	timer -= 1;
	if(timer == 0){
		for(uint8_t k = 0; k < 3; k++){
			lcd_fill(COLOR_RED);
			delay_ms(100);
			lcd_fill(COLOR_BLACK);
			delay_ms(100);
		}
		
		game_init();
	}
		

		
	lcd_cursor(0, 0);
	
	char buff[16];
	sprintf(buff, "S: %03i, T: %03i", score, timer);
	lcd_println((uint8_t*)buff);
}
