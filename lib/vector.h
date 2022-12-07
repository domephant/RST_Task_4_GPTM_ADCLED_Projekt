#ifndef VECTOR_H
	#define VECTOR_H
	#include <stdint.h>

	#define vec2(x, y) ((vec2_t){(x), (y)})
	
	typedef struct
	{
			int8_t x;
			int8_t y;
	} vec2_t;
	
	/**
	 * @brief Adds two vectors and returns their element-wise sum.
	 */
	vec2_t vec2_add(vec2_t a, vec2_t b);

	/**
	 * @brief Subtracts two vectors and returns their element-wise sum.
	 */
	vec2_t vec2_sub(vec2_t a, vec2_t b);

	/**
	 * @brief Returns a vector with the elements of the given vector multiplied with the given scalar.
	 */
	vec2_t vec2_mul(vec2_t v, int8_t s);

	/**
	 * @brief Returns a vector that is turned 90 degrees to the left from the given vector.
	 */
	vec2_t vec2_left(vec2_t v);

	/**
	 * @brief Returns a vector that is turned 90 degrees to the right from the given vector.
	 */
	vec2_t vec2_right(vec2_t v);

	/**
	 * @brief Multiplies the x vector with the x component of v adn the same with y. Returns the sum of both.
	 */
	vec2_t vec2_apply(vec2_t x, vec2_t y, vec2_t v);
#endif
