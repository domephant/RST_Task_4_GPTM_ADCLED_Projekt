#ifndef ADCLED_H
#define ADCLED_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib/tm4c123gh6pm.h"
#include "TM4C123GH6PM.h"
#include "lib/lcd.h"

typedef struct {
	uint16_t x;
	uint16_t y;
} joy_val;

typedef struct {
	uint16_t x;
	uint16_t y;
} light_val;

void joystick_calc(joy_val *pos, light_val *light);

#endif // ADCLED
