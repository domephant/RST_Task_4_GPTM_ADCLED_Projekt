#ifndef TRAFFICLIGHT_H
	#define TRAFFICLIGHT_H
	#include "lcd.h"
	#include "vector.h"

	#define TL_NUM_LIGHTS 4

	#define TL_RADIUS 5
	#define TL_PITCH 12
	#define TL_PADDING 2
	#define TL_FONTSIZE 8
	#define TL_WIDTH ((2*(TL_RADIUS + TL_PADDING) + 1))
	#define TL_HEIGHT ((3*TL_PITCH)+ TL_PADDING)
	#define PL_WIDTH TL_WIDTH
	#define PL_HEIGHT ((2*TL_PITCH) + TL_PADDING)
	#define COLOR_TL_BG       RGB565(0x20, 0x20, 0x30)
	#define COLOR_RED_DARK    RGB565(0x30, 0x00, 0x00)
	#define COLOR_ORANGE_DARK RGB565(0x30, 0x2D, 0x00)
	#define COLOR_GREEN_DARK  RGB565(0x00, 0x30, 0x00)

	typedef enum
	{
		TL_STATE_OFF,
		TL_STATE_RED,
		TL_STATE_REDORANGE,
		TL_STATE_GREEN,
		TL_STATE_ORANGE
	} TL_STATE;

	#define TL_DIR_NORTH vec2( 0,  1)
	#define TL_DIR_EAST  vec2(-1,  0)
	#define TL_DIR_SOUTH vec2( 0, -1)
	#define TL_DIR_WEST  vec2( 1,  0)

	typedef struct
	{
		uint8_t id;
		uint8_t is_pedestrian_light;
		vec2_t pos; // position
		vec2_t dir; // direction
		TL_STATE state;
	} trafficlight_t;

	extern trafficlight_t tlights[TL_NUM_LIGHTS];

	void tl_init(void);

	/**
	 * @brief Sets the state of one \struct trafficlight_t given by \enum state and draws it to the display.  
	 * 
	 * @param this A pointer to the \struct trafficlight_t which has to be manipulated.
	 * @param state The \enum TL_STATE to be set.
	 */
	void tl_set_state(trafficlight_t *this, TL_STATE state);
	void tl_set_phase(uint8_t id, TL_STATE phase);
#endif
