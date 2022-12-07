#include "maze_game.h"
#include "stdint.h"
#include "lcd.h"
#include <stdio.h>
#include "stdlib.h"
#include "delay.h"
#include "maze_generator.h"

#define block_size 5
#define maze_pos_x 11
#define maze_pos_y 16
#define radius 2

double pos_x, pos_y;
double pos_x_old, pos_y_old;
double speed_x, speed_y;

uint8_t maze[maze_height][maze_width];

float absf(float val){
	if(val < 0){
		return -val;
	}else{
		return val;
	}
}



void game_init(){
	
	speed_x = 0.0;
	speed_y = 0.0;
	
	lcd_fill(COLOR_BLACK);
	
	
	int maze_start_x;
	int maze_start_y;
	int maze_end_x;
	int maze_end_y;
	
	generate_maze((char *) maze, &maze_start_x, &maze_start_y, &maze_end_x, &maze_end_y);
	
	
	//pos_x = (rand() % (maze_width / 2)) * 2 + 1;
	//pos_y = (rand() % (maze_height / 2)) * 2 + 1;
	
	//use coordinates of the maze end point instead of random values
	//convert maze coordinates into screen coordinates
	pos_x = maze_pos_x + block_size * maze_end_x + 2;
	pos_y = maze_pos_y + block_size * maze_end_y + 2;
	
	//display maze on the screen
	lcd_rectangle_fill(maze_pos_x + block_size * maze_start_x,
										 maze_pos_y + block_size * maze_start_y,
										 maze_pos_x + block_size * (maze_start_x + 1),
										 maze_pos_y + block_size * (maze_start_y + 1),
										 COLOR_GREEN);
										 
	lcd_circle_fill(pos_x, pos_y, radius, RGB565(0xFF, 0x00, 0xFF));
	
	for (int y = 0; y < maze_height; y++) {
		for (int x = 0; x < maze_width; x++) {
			if (maze[y][x] == '#')
				lcd_rectangle_fill(maze_pos_x + block_size * x,
													 maze_pos_y + block_size * y,
													 maze_pos_x + block_size * (x + 1),
													 maze_pos_y + block_size * (y + 1),
													 COLOR_RED);
		}
	}
}


void game_update(int8_t x, int8_t y){
	
	rand();
	lcd_circle_fill(pos_x, pos_y, 2, COLOR_BLACK);
	
	//double pos_x_old, pos_y_old;
	
	uint8_t collision;
	uint8_t restart;
	uint8_t temp;
	
	speed_x += x / 2000.0;
	speed_y += y / 2000.0;
	
	//check for collisions
	pos_x_old = pos_x;
	pos_y_old = pos_y;
	
	//check if one of the four corners of the ball interpreted as a square collide with the walls
	
	//x-axis collisions
	
	collision = 0;
	restart = 0;
	pos_x += speed_x;
	
	//left top corner
	temp = maze[(uint8_t) ((pos_y - maze_pos_y - radius) / block_size)][(uint8_t) ((pos_x - maze_pos_x - radius) / block_size)];
	if (temp != ' ')
		collision = 1;
	if (temp == 'S')
		restart = 1;
	//right top corner
	temp = maze[(uint8_t) ((pos_y - maze_pos_y - radius) / block_size)][(uint8_t) ((pos_x - maze_pos_x + radius) / block_size)];
	if (temp != ' ')
		collision = 1;
	if (temp == 'S')
		restart = 1;
	//right bottom corner
	temp = maze[(uint8_t) ((pos_y - maze_pos_y + radius) / block_size)][(uint8_t) ((pos_x - maze_pos_x + radius) / block_size)];
	if (temp != ' ')
		collision = 1;
	if (temp == 'S')
		restart = 1;
	//left bottom corner
	temp = maze[(uint8_t) ((pos_y - maze_pos_y + radius) / block_size)][(uint8_t) ((pos_x - maze_pos_x - radius) / block_size)];
	if (temp != ' ')
		collision = 1;
	if (temp == 'S')
		restart = 1;
	
	if (collision) {
		pos_x = pos_x_old;
		speed_x = 0;
	}
	
	
	//y-axis collisions
	
	collision = 0;
	pos_y += speed_y;
	
	//left top corner
	temp = maze[(uint8_t) ((pos_y - maze_pos_y - radius) / block_size)][(uint8_t) ((pos_x - maze_pos_x - radius) / block_size)];
	if (temp != ' ')
		collision = 1;
	if (temp == 'S')
		restart = 1;
	//right top corner
	temp = maze[(uint8_t) ((pos_y - maze_pos_y - radius) / block_size)][(uint8_t) ((pos_x - maze_pos_x + radius) / block_size)];
	if (temp != ' ')
		collision = 1;
	if (temp == 'S')
		restart = 1;
	//right bottom corner
	temp = maze[(uint8_t) ((pos_y - maze_pos_y + radius) / block_size)][(uint8_t) ((pos_x - maze_pos_x + radius) / block_size)];
	if (temp != ' ')
		collision = 1;
	if (temp == 'S')
		restart = 1;
	//left bottom corner
	temp = maze[(uint8_t) ((pos_y - maze_pos_y + radius) / block_size)][(uint8_t) ((pos_x - maze_pos_x - radius) / block_size)];
	if (temp != ' ')
		collision = 1;
	if (temp == 'S')
		restart = 1;
	
	if (restart) {
		for(uint8_t k = 0; k < 1; k++){
			lcd_fill(COLOR_RED);
			delay_ms(100);
			lcd_fill(COLOR_BLACK);
			delay_ms(100);
		}
		game_init();
		return;
	}
	else if (collision) {
		pos_y = pos_y_old;
		speed_y = 0;
	}
	
	
	lcd_circle_fill(pos_x, pos_y, radius, RGB565(0xFF, 0x00, 0xFF));
	
	lcd_cursor(0, 0);
	
	char buff[16];
	sprintf(buff, "X: %4i, Y: %4i", x, y);
	lcd_println((uint8_t*)buff);
	
}
