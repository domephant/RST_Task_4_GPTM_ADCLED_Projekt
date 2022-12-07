#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdlib.h>
#include "gpio.h"
#include "delay.h"
#include "tm4c123gh6pm.h"
/**
	* Color Macros and Constants
	*/
#define RGB565(R,G,B) ((uint16_t)((((R) / 8) << 11) + (((G) / 4) << 5) + ((B) / 8)))  
#define COLOR_RED		RGB565(0xFF, 0x00, 0x00)
#define COLOR_GREEN	RGB565(0x00, 0xFF, 0x00)
#define COLOR_ORANGE RGB565(0xFF, 0xF0, 0x00)
#define COLOR_BLUE	RGB565(0x00, 0xFF, 0x00)
#define COLOR_WHITE	RGB565(0xFF, 0xFF, 0xFF)
#define COLOR_BLACK	RGB565(0x00, 0x00, 0x00)

/**
	*	LCD Controller Bus GPIOs
	*/
#define LCD_SCK_PORT		PORTB
#define LCD_SCK_PIN			4
#define LCD_MOSI_PORT 	PORTB
#define LCD_MOSI_PIN  	7
#define LCD_CS_PORT			PORTA
#define LCD_CS_PIN			4
#define LCD_RESET_PORT	PORTF
#define LCD_RESET_PIN		0
#define LCD_DS_PORT			PORTF
#define LCD_DS_PIN			4

#define LCD_USE_HW_SPI

/**
	*	LCD Controller Constants
	*/
#define HX8353E_WIDTH  128
#define HX8353E_HEIGHT 128
#define HX8353E_MADCTL_MY  0x80
#define HX8353E_MADCTL_MX  0x40
#define HX8353E_MADCTL_MV  0x20
#define HX8353E_MADCTL_ML  0x10
#define HX8353E_MADCTL_RGB 0x08
#define HX8353E_MADCTL_MH  0x04

/**
	*	LCD Controller Registers
	*/
#define HX8353E_NOP     0x00
#define HX8353E_SWRESET 0x01
#define HX8353E_RDDID   0x04
#define HX8353E_RDDST   0x09
#define HX8353E_SLPIN   0x10
#define HX8353E_SLPOUT  0x11
#define HX8353E_PTLON   0x12
#define HX8353E_NORON   0x13
#define HX8353E_INVOFF  0x20
#define HX8353E_INVON   0x21
#define HX8353E_GAMSET  0x26
#define HX8353E_DISPOFF 0x28
#define HX8353E_DISPON  0x29
#define HX8353E_CASET   0x2A
#define HX8353E_RASET   0x2B
#define HX8353E_RAMWR   0x2C
#define HX8353E_RGBSET  0x2d
#define HX8353E_RAMRD   0x2E
#define HX8353E_PTLAR   0x30
#define HX8353E_MADCTL  0x36
#define HX8353E_COLMOD  0x3A
#define HX8353E_SETPWCTR 0xB1
#define HX8353E_SETDISPL 0xB2
#define HX8353E_FRMCTR1  0xB1
#define HX8353E_FRMCTR2  0xB2
#define HX8353E_FRMCTR3  0xB3
#define HX8353E_SETCYC   0xB4
#define HX8353E_SETBGP   0xb5
#define HX8353E_SETVCOM  0xB6
#define HX8353E_SETSTBA  0xC0
#define HX8353E_SETID    0xC3
#define HX8353E_GETHID   0xd0
#define HX8353E_SETGAMMA 0xE0


/**
	* LCD initialization routine
	* - sets up gpio in order to provide SPI communication to the display
	* - initializes the display with a startup sequence
	* - can also be used to reset the display
  */
void lcd_init(void);

/**
	* Fill routine
	* - fills the entire screen with given rgb color value
	*
	* @param	color		fill color in RGB565 format
	*/
void lcd_fill(uint16_t color);

/**
	* Pixel drawing routine
	* - Sets the color value of a pixel on the lcd
	*
	* @param	x		x-coordinate of the pixel
	* @param	y		y-coordinate of the pixel
	* @param	color		pixel color om RGB565 format
	*/
void lcd_pixel(uint8_t x, uint8_t y, uint16_t color);

/**
	* Line drawing routine
	* - Draws a 1px line between x0,y0 and x1,y1
	*
	* @param	x0		x-coordinate of the start point
	* @param	y0		y-coordinate of the start point
	* @param	x1		x-coordinate of the end point
	* @param	y1		y-coordinate of the end point
	* @param	color		pixel color om RGB565 format
	*/
void lcd_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);

/**
	* Line drawing routine
	* - Draws a 1px line between x0,y0 and x1,y1
	*
	* @param	x0				x-coordinate of the start point
	* @param	y0				y-coordinate of the start point
	* @param	x1				x-coordinate of the end point
	* @param	y1				y-coordinate of the end point
	* @param  len_line	length of one dash
	* @param	color		 	pixel color om RGB565 format
	*/
void lcd_dashed_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t len_line, uint16_t color);

/**
	* Rectangle drawing routine
	* - Draws a 1px rectangle defined by the two points x0,y0 and x1,y1
	*
	* @param	x0		x-coordinate of the first point
	* @param	y0		y-coordinate of the first point
	* @param	x1		x-coordinate of the second point
	* @param	y1		y-coordinate of the second point
	* @param	color		pixel color om RGB565 format
	*/
void lcd_rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);

/**
	* Filled rectangle drawing routine
	* - Draws a filled rectangle defined by the two points x0,y0 and x1,y1
	*
	* @param	x0		x-coordinate of the first point
	* @param	y0		y-coordinate of the first point
	* @param	x1		x-coordinate of the second point
	* @param	y1		y-coordinate of the second point
	* @param	color		pixel color om RGB565 format
	*/
void lcd_rectangle_fill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color);
	
/**
	* Circle drawing routine
	* - Draws a 1px circle outline with given color values
	*
	* @param 	x0		x-coordinate of circle center
	* @param	y0		y-coordinate of circle center
	* @param	radius		radius of circle
	* @param	color		circle color in RGB565 format
	*/
void lcd_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color);

/**
	* Filled circle drawing routine
	* - Draws a filled circle outline with given color values
	*
	* @param 	x0		x-coordinate of circle center
	* @param	y0		y-coordinate of circle center
	* @param	radius		radius of circle
	* @param	color		circle color in RGB565 format
	*/
void lcd_circle_fill(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color);

/**
	*	Character drawing routine
	* - Draws a given ASCII character on the given position with a specific foreground and background color
	*
	* @param	x		x-coordinate for the character to be drawn (origin: upper-left)
	* @param	y		y-coordinate for the character to be drawn (origin: upper-left)
	* @param	ch	character to be drawn (possible values 0-127; basic ascii)
	* @param	colorf		foreground color in RGB565 format
	* @param	colorb		background color in RGB565 format
	*/
void lcd_character(uint8_t x, uint8_t y, uint8_t ch, uint16_t colorf, uint16_t colorb);

/**
	*	Character sequence drawing routine
	* - Draws a given character sequence (text) with a specific foreground and background color
	* - The position can be specified with lcd_cursor(x, y)
	* - The cursor position is updated when drawing text with lcd_print_rgb(), lcd_println_rgb(), lcd_print() and lcd_println()
	* - Instead of drawing the \n character the cursor y position is increased
	* 
	* @param	ch		character sequence
	* @param	colorf		foreground color in RGB565 format
	* @param	colorb		background color in RGB565 format
	*/
void lcd_print_rgb(uint8_t* ch, uint16_t colorf, uint16_t colorb);
/**
	*	Character sequence drawing routine with newline suffix
	* - Draws a given character sequence (text) with a specific foreground and background color
	* - The position can be specified with lcd_cursor(x, y)
	* - The cursor position is updated when drawing text with lcd_print_rgb(), lcd_println_rgb(), lcd_print() and lcd_println()
	* - Instead of drawing the \n character the cursor y position is increased
	* - After drawing the sequence the cursor y position is increased
	* 
	* @param	ch		character sequence
	* @param	colorf		foreground color in RGB565 format
	* @param	colorb		background color in RGB565 format
	*/
void lcd_println_rgb(uint8_t* ch, uint16_t colorf, uint16_t colorb);

/**
	*	Character sequence drawing routine withoud color
	* - Draws a given character sequence (text) with white foreground color and black background color
	* - The position can be specified with lcd_cursor(x, y)
	* - The cursor position is updated when drawing text with lcd_print_rgb(), lcd_println_rgb(), lcd_print() and lcd_println()
	* - Instead of drawing the \n character the cursor y position is increased
	*
	* @param	ch		character sequence
	*/
void lcd_print(uint8_t* ch);

/**
	*	Character sequence drawing routine without color
	* - Draws a given character sequence (text) with white foreground color and black background color
	* - The position can be specified with lcd_cursor(x, y)
	* - The cursor position is updated when drawing text with lcd_print_rgb(), lcd_println_rgb(), lcd_print() and lcd_println()
	* - Instead of drawing the \n character the cursor y position is increased
	* - After drawing the sequence the cursor y position is increased
	*
	* @param	ch		character sequence
	*/
void lcd_println(uint8_t* ch);

/**
	* Cursor setting routine
	* - Sets the character drawing cursor to the specified position
	*
	* @param	x		x-coordinate for the cursor (origin: upper-left)
	* @param	y		y-coordinate for the cursor (origin: upper-left)	
	*/
void lcd_cursor(uint8_t x, uint8_t y);

/**
	* Bitmap drawing routine
	* - draws a bitmap at the specified position
	* - pixel data is given as uint16_t array (RGB565)
	* - pixel data length must be width*height
	*
	* @param	x		x-coordinate for the bitmap to be drawn (origin: upper-left)
	* @param	y		y-coordinate for the bitmap to be drawn (origin: upper-left)
	* @param	pixmap	uint16_t array of bitmap data (RGB565)
	* @param 	width		width of bitmap
	* @param	height	height of bitmap
	*/
void lcd_bitmap(uint8_t x, uint8_t y, uint16_t* bitmap, uint8_t width, uint8_t height);

/**
	* Bitmap drawing routine
	* - draws a bitmap at the specified position
	* - pixel data is given as uint16_t array (RGB565)
	* - pixel data length must be width*height
	*
	* @param	x		x-coordinate for the bitmap to be drawn (origin: upper-left)
	* @param	y		y-coordinate for the bitmap to be drawn (origin: upper-left)
	* @param	pixmap	uint16_t array of bitmap data (RGB565)
	* @param 	width		width of bitmap
	* @param	height	height of bitmap
	* @param 	alphacolor	color key for alpha (RGB565)
	*/
void lcd_bitmap_alpha(uint8_t x, uint8_t y, uint16_t* bitmap, uint8_t width, uint8_t height, uint16_t alphacolor);

#endif
