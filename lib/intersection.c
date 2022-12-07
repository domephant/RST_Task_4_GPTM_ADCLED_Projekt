#include "intersection.h"

void intersection_init(void)
{
	lcd_fill(COLOR_BLACK);
	
	//vertical
	lcd_line(50,50,50,0,COLOR_WHITE); //upper left
	lcd_line(77,50,77,0,COLOR_WHITE); //upper right
	lcd_line(50,77,50,127,COLOR_WHITE); //lower left
	lcd_line(77,77,77,127,COLOR_WHITE); //lower right
	
	//horizontal
	lcd_line(0,50,50,50,COLOR_WHITE); //left upper
	lcd_line(77,50,127,50,COLOR_WHITE); //right upper
	lcd_line(0,77,50,77,COLOR_WHITE); //left lower
	lcd_line(77,77,127,77,COLOR_WHITE); //right lower
	
	//draw stoplines
	lcd_line(52,50,64,50,COLOR_WHITE);
	lcd_line(64,77,75,77,COLOR_WHITE);
	lcd_line(50,65,50,75,COLOR_WHITE);
	lcd_line(77,52,77,64,COLOR_WHITE);
	
	//draw dashed lines
	lcd_dashed_line(64,50,64,0,8,COLOR_ORANGE);
	lcd_dashed_line(0,64,50,64,8,COLOR_ORANGE);
	lcd_dashed_line(64,127,64,77,8,COLOR_ORANGE);
	lcd_dashed_line(77,64,127,64,8,COLOR_ORANGE);
}
