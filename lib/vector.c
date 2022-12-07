#include "vector.h"

vec2_t vec2_add(vec2_t a, vec2_t b)
{
	vec2_t result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
	return result;
}

vec2_t vec2_sub(vec2_t a, vec2_t b)
{
	vec2_t result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
	return result;
}

vec2_t vec2_mul(vec2_t v, int8_t s)
{
	vec2_t result;
  result.x = v.x * s;
  result.y = v.y * s;
	return result;
}

vec2_t vec2_left(vec2_t v)
{
	vec2_t result;
  result.x =  v.y;
  result.y = -v.x;
	return result;
}

vec2_t vec2_right(vec2_t v)
{
	vec2_t result;
  result.x = -v.y;
  result.y =  v.x;
	return result;
}

vec2_t vec2_apply(vec2_t x, vec2_t y, vec2_t v)
{
	return vec2_add(vec2_mul(x, v.x), vec2_mul(y, v.y));
}
