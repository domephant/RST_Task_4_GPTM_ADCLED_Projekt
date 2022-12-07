#ifndef MARIO_H
#define MARIO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "delay.h"
#include "lcd.h"


#define MAX_GUMPA		16

/**
	* Basic mario structure
	*
	* @param 	x									x-coordinate of mario
	* @param	y									y-coordinate of mario
	* @param	vx								x speed of mario
	*	@param	vy								y speed of mario
	*	@param	last_direction		last direction (last frame) of mario
	*/
typedef struct {
	float x;
	float y;
	float vx;
	float vy;
	
	uint8_t last_direction;
} mario_t;


/**
	* Basic input structure
	*
	* @param 	joystick					current value of the joystick (-1 <= joystick <= 1)
	* @param	jump							jump button pressed
	*/
typedef struct {
	float joystick;
	uint8_t jump;
} input_t;


/**
	* Basic game structure
	*
	* @param 	mario					mario object
	* @param	input					input object
	* @param	score					current score
	* @param	gumpa_num			current count of gumpas
	*	@param	gumpa					array containing x-coordinates of gumpas
	*/
typedef struct {
	mario_t mario;
	input_t input;
	
	uint16_t score;
	
	uint16_t gumpa_num;
	float gumpa[MAX_GUMPA];
} game_t;


/**
	* Initialization routine
	*
	*/
void mario_init(void);

/**
	* Input routine, must be called once per frame
	*
	* @param 	x			x-controll value -1 <= x <= 1
	* @param	jump	jump button state
	*/
void mario_input(float x, uint8_t jump);

/**
	* Frame routine
	* - Draws the frame and handles logic
	*
	*/
void mario_loop(void);

#endif
