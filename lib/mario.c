#include "mario.h"
#include "mario_bitmaps.h"


game_t game;

char buff[256];

void spawn_gumpa(){
	game.gumpa[game.gumpa_num] = rand() % 2 ? 0 : 128-16;
	game.gumpa_num++;
}

void mario_init(){
	lcd_rectangle_fill(0, 0, 16*8, 16*7, RGB565(135,206,250));
	
	game.mario.x = 64-6;
	game.mario.y = 0;
	
	game.mario.last_direction = 0;
	game.mario.vx = 0;
	game.mario.vy = 0;
	
	game.score = 0;
	game.gumpa_num = 0;
	
	spawn_gumpa();
}

void mario_input(float x, uint8_t jump){
	game.input.joystick = x;
	game.input.jump = jump;
}

void mario_loop(){
	uint8_t right;
	uint8_t left;
	
	if( game.input.joystick > 0.2f ){
		right = 0;
	}else{
		right = 1;
	}
	
	if( game.input.joystick < -0.2f ){
		left = 0;
	}else{
		left = 1;
	}
	
	
	if(!left || !right){
		if(!right){
			game.mario.vx += 1;
			game.mario.last_direction = 0;
		}
		if(!left){
			game.mario.vx -= 1;
			game.mario.last_direction = 1;
		}
	}else{
		game.mario.vx *= 0.2f;
	}
	
	game.mario.vy -= 0.8f;
	if(game.mario.y <= 0){
		game.mario.y = 0;
		game.mario.vy = 0;
	}
	
	game.mario.vx = fmin(5, fmax(-5, game.mario.vx));
	game.mario.x = fmin(128-6, fmax(0, game.mario.x));
	
	
	if( game.input.jump && game.mario.y == 0 ){
		game.mario.vy = 8;
	}
	
	lcd_rectangle_fill((uint8_t)game.mario.x, (uint8_t)(6*16 - game.mario.y), (uint8_t)game.mario.x + 12, (uint8_t)(6*16 - game.mario.y + 16), RGB565(135,206,250));
	game.mario.x += game.mario.vx;
	game.mario.y += game.mario.vy;
	
	for(int i = 0; i < game.gumpa_num; i++){
		lcd_rectangle_fill((uint8_t)game.gumpa[i], (uint8_t)(6*16), (uint8_t)game.gumpa[i] + 16, (uint8_t)(6*16+16), RGB565(135,206,250));
		
		if(game.mario.x+6 > game.gumpa[i] + 8){
			game.gumpa[i] += 1;
		}else{
			game.gumpa[i] -= 1;
		}
		
		if(((game.mario.x + 12 > game.gumpa[i] && game.mario.x + 12 < game.gumpa[i] + 16 ) || (game.mario.x > game.gumpa[i] && game.mario.x < game.gumpa[i] + 16 ) )&& game.mario.y < 16){
			// Colision
			if( game.mario.vy < 0 ){
				// Won
				game.score += 1;
				game.gumpa[i] = game.gumpa[game.gumpa_num-1];
				game.gumpa_num -= 1;
				
				lcd_rectangle_fill(0, 0, 16*8, 16*7, RGB565(135,206,250));
				
				spawn_gumpa();
			}else{
				// Loose 
				mario_init();
			}
			
		}
		
		lcd_bitmap_alpha((uint8_t)game.gumpa[i], (uint8_t)(6*16), (uint16_t*)bitmap_gumpa, 16, 16, COLOR_RED);
	}

	if(!game.mario.last_direction)
		lcd_bitmap_alpha((uint8_t)game.mario.x, (uint8_t)(6*16 - game.mario.y), (uint16_t*)bitmap_mario_r, 12, 16, COLOR_BLACK);
	else
		lcd_bitmap_alpha((uint8_t)game.mario.x, (uint8_t)(6*16 - game.mario.y), (uint16_t*)bitmap_mario_l, 12, 16, COLOR_BLACK);	

	for(int i = 0; i < 8; i++){
		lcd_bitmap(i*16, 7*16, (uint16_t*)bitmap_floor, 16, 16);
	}

	lcd_cursor(0,0);
	sprintf (buff, "Score: %i", game.score);
	lcd_println((uint8_t*)buff);

	delay_ms(4);
}
