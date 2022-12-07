#include <stdlib.h>
#include "maze_generator.h"
#include "stdint.h"

#define xy(y, x) ((y) * maze_width + (x))

int generate_maze(char *maze, int *start_pos_x, int *start_pos_y, int *end_pos_x, int *end_pos_y) {
	if (maze_width % 2 == 0 || maze_height % 2 == 0) {
		return 1;
	}
	
	//initialize the maze with squares
	for (int y = 0; y < maze_height; y++) {
		for (int x = 0; x < maze_width; x++) {
			maze[xy(y, x)] = (y % 2 == 0 || x % 2 == 0) ? '#' : 'o';
		}
	}
	
	//remove walls using randomized depth-first search
	
	int direction;
	/*
	0: left
	1: up
	2: right
	3: down*/
	
	uint32_t stack_size = maze_width * maze_height / 4;
	uint8_t stack[maze_width * maze_height / 4];
	int stack_pointer = -1;
	
	uint8_t stack_copy[maze_width * maze_height / 4];
	int max_path_length = 0;
	
	int x;
	int y;
	
	//the initial direction determines the wall at which the generatir starts
	direction = rand() % 4;
	
	if (direction == 0 || direction == 2)
		*start_pos_y = (rand() % ((maze_height - 2) / 2)) * 2 + 1;
	else
		*start_pos_x = (rand() % ((maze_width  - 2) / 2)) * 2 + 1;
	
	switch (direction) {
		case 0:
			*start_pos_x = maze_width - 2;
			//maze[xy(*start_pos_y, *start_pos_x + 1)] = ' ';
			break;
		case 1:
			*start_pos_y = maze_height - 2;
			//maze[xy(*start_pos_y + 1, *start_pos_x)] = ' ';
			break;
		case 2:
			*start_pos_x = 1;
			//maze[xy(*start_pos_y, *start_pos_x - 1)] = ' ';
			break;
		case 3:
			*start_pos_y = 1;
			//maze[xy(*start_pos_y  - 1, *start_pos_x)] = ' ';
			break;
	}
	
	x = *start_pos_x;
	y = *start_pos_y;
	
	//mark cell as visited and push it to the stack
	maze[xy(y, x)] = 'S';
	stack[0] = direction;
	stack_pointer++;
	
	int neighbour_count;
	int neighbours[4];
	int x_old;
	int y_old;
	
	while (stack_pointer >= 0 && stack_pointer < stack_size) {
		
		//check if the current cell has unvisited neighbours
		neighbour_count = 0;
		//left
		if (x > 2 && maze[xy(y, x - 2)] == 'o') {
			neighbours[neighbour_count] = 0;
			neighbour_count++;
		}
		//up
		if (y > 2 && maze[xy(y - 2, x)] == 'o') {
			neighbours[neighbour_count] = 1;
			neighbour_count++;
		}
		//right
		if (x < maze_width - 3 && maze[xy(y, x + 2)] == 'o') {
			neighbours[neighbour_count] = 2;
			neighbour_count++;
		}
		//down
		if (y < maze_height - 3 && maze[xy(y + 2, x)] == 'o') {
			neighbours[neighbour_count] = 3;
			neighbour_count++;
		}
		
		//select a random neighbour cell if possible
		if (neighbour_count > 0) {
			direction = neighbours[rand() % neighbour_count];
			
			//push new cell to the stack
			stack[stack_pointer] = direction;
			stack_pointer++;
			//copy stack if current stack is bigger than the last maximum
			if (stack_pointer > max_path_length) {
				for (int i = 0; i < stack_pointer; i++)
					stack_copy[i] = stack[i];
				max_path_length = stack_pointer;
			}
			//save old position
			x_old = x;
			y_old = y;
			
			//move cursor
			if (direction % 2 == 0)
				x += (direction == 0) ? (-2) : (+2);
			else if (direction % 2 == 1)
				y += (direction == 1) ? (-2) : (+2);
			//remove wall between old and new cell
			maze[xy((y + y_old) / 2, (x + x_old) / 2)] = ' ';
			//mark new cell as visited
			maze[xy(y, x)] = ' ';
		}
		//if there's no unvisited neighbour then backtrack
		else {
			//pop cell from the stack
			stack_pointer--;
			direction = stack[stack_pointer];
			
			//move cursor
			if (direction % 2 == 0)
				x += (direction == 0) ? (+2) : (-2);
			else if (direction % 2 == 1)
				y += (direction == 1) ? (+2) : (-2);
		}
			
	}
	
	//search the end point (longest path from the starting point) of the maze
	x = *start_pos_x;
	y = *start_pos_y;
	
	for (int i = 0; i < max_path_length; i++) {
		direction = stack_copy[i];
		//move cursor
		if (direction % 2 == 0)
			x += (direction == 0) ? (-2) : (+2);
		else if (direction % 2 == 1)
			y += (direction == 1) ? (-2) : (+2);
	}
	
	*end_pos_x = x;
	*end_pos_y = y;
	
	//maze[xy(y, x)] = 'F';
	
	return 0;
}
