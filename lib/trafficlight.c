#include "trafficlight.h"

trafficlight_t tlights[TL_NUM_LIGHTS];

//vectors to be added to pos of each sunshade
vec2_t tl_size = { TL_HEIGHT, TL_WIDTH };
vec2_t pl_size = { PL_HEIGHT, PL_WIDTH };

vec2_t red    = { TL_PADDING + TL_RADIUS,                (TL_WIDTH >> 1)};
vec2_t orange = { TL_PADDING + TL_RADIUS + TL_PITCH,     (TL_WIDTH >> 1)};
vec2_t green  = { TL_PADDING + TL_RADIUS + (TL_PITCH << 1), (TL_WIDTH >> 1)};

void tl_draw_sunshade(const trafficlight_t *this)
{
	vec2_t a, b; // corners of the rectangle we want to fill
	vec2_t x, y; // direction vectors
	vec2_t c; // text cursor position
	a = this->pos;
	x = this->dir;
	y = vec2_left(x);
	b = vec2_add(a, vec2_apply(x, y, this->is_pedestrian_light ? pl_size : tl_size));

	lcd_rectangle_fill(a.x, a.y, b.x, b.y, COLOR_TL_BG); //draw sunshade
	
	c = vec2_add(b, vec2_sub(vec2_mul(x, (TL_FONTSIZE >> 1)), vec2_mul(y, (pl_size.y >> 1))));

	lcd_cursor(c.x - TL_FONTSIZE / 2, c.y - (TL_FONTSIZE >> 1));
	uint8_t id[2] = {this->id + '0', 0};
	lcd_print(id);
}

void tl_set_light_coordinates_tl(const trafficlight_t *this, vec2_t *lights)
{
	vec2_t x, y;
	x = this->dir;
	y = vec2_left(x);
	lights[0] = vec2_add(this->pos, vec2_apply(x, y,    red));
	lights[1] = vec2_add(this->pos, vec2_apply(x, y, orange));
	lights[2] = vec2_add(this->pos, vec2_apply(x, y,  green));
}

void tl_set_light_coordinates_pl(const trafficlight_t *this, vec2_t lights[])
{
	vec2_t x, y;
	x = this->dir;
	y = vec2_left(x);
	lights[0] = vec2_add(this->pos, vec2_apply(x, y,    red));
	lights[1] = vec2_add(this->pos, vec2_apply(x, y, orange));
}

void tl_set_state(trafficlight_t *this, TL_STATE state)
{
	uint16_t colors[3];
	vec2_t lights[3];
	if (this->is_pedestrian_light)
	{
		tl_set_light_coordinates_pl(this, lights);
	}
	else
	{
		tl_set_light_coordinates_tl(this, lights);
	}

	switch (state)
	{
	case TL_STATE_OFF:
		colors[0] = COLOR_RED_DARK;
		colors[1] = COLOR_ORANGE_DARK;
		colors[2] = COLOR_GREEN_DARK;
		break;

	case TL_STATE_RED:
		colors[0] = COLOR_RED;
		colors[1] = COLOR_ORANGE_DARK;
		colors[2] = COLOR_GREEN_DARK;
		break;

	case TL_STATE_REDORANGE:
		colors[0] = COLOR_RED;
		colors[1] = COLOR_ORANGE;
		colors[2] = COLOR_GREEN_DARK;
		break;

	case TL_STATE_GREEN:
		colors[0] = COLOR_RED_DARK;
		colors[1] = COLOR_ORANGE_DARK;
		colors[2] = COLOR_GREEN;
		break;

	case TL_STATE_ORANGE:
		colors[0] = COLOR_RED_DARK;
		colors[1] = COLOR_ORANGE;
		colors[2] = COLOR_GREEN_DARK;
		break;
	}
	this->state = state;

	lcd_circle_fill(lights[0].x, lights[0].y, TL_RADIUS, colors[0]);
	if (this->is_pedestrian_light)
	{
		lcd_circle_fill(lights[1].x, lights[1].y, TL_RADIUS, colors[2]);
	}
	else
	{
		lcd_circle_fill(lights[1].x, lights[1].y, TL_RADIUS, colors[1]);
		lcd_circle_fill(lights[2].x, lights[2].y, TL_RADIUS, colors[2]);
	}
}

void tl_set_phase(uint8_t id, TL_STATE phase)
{
	tl_set_state(&tlights[id], phase);
}

void tl_init(void)
{
	
	tlights[0].dir =  TL_DIR_EAST;
	tlights[0].pos =  vec2(47, 80);

	tlights[1].dir = TL_DIR_NORTH;
	tlights[1].pos = vec2(80, 80);

	tlights[2].dir = TL_DIR_WEST;
	tlights[2].pos = vec2(80, 47);

	tlights[3].dir = TL_DIR_SOUTH;
	tlights[3].pos = vec2(47, 47);

	for (uint8_t i = 0; i < TL_NUM_LIGHTS; i++)
	{
		tlights[i].id = i;
		tlights[i].state = TL_STATE_ORANGE;
		//tlights[i].is_pedestrian_light = (i & 1);
		if (tlights[i].is_pedestrian_light)
		{
			tlights[i].pos = vec2_add(tlights[i].pos, vec2_mul(vec2_left(tlights[i].dir), TL_WIDTH));
		}
		tl_draw_sunshade(&tlights[i]);
		tl_set_state(&tlights[i], TL_STATE_OFF);
	}
}
