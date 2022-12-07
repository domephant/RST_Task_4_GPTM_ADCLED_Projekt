#include "main.h"

#define ADC_OFFSET 	2048

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define SQRT_SCALE 	900
#define SQRT_SCALE2 30		//must be sqrt(SQRT_SCALE)

/*
* LUT for modeling the relation adc-value/perceived brightness->
* pwm-value/actual brightness
*
* computed according to x = b(p/b+1)^(y/r)-b with
* x				apparent brightness
* y				actual brightness
* b=3, 			scaling factor
* p=0x10000,	the pwm resolution/period
* r=0x1000, 	the adc resolution
*/
uint16_t pwm_16[257] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0001, 0x0001, 0x0001,
	0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002,
	0x0003, 0x0003, 0x0003, 0x0003, 0x0004, 0x0004, 0x0004, 0x0004,
	0x0005, 0x0005, 0x0005, 0x0006, 0x0006, 0x0006, 0x0007, 0x0007,
	0x0007, 0x0008, 0x0008, 0x0009, 0x0009, 0x000A, 0x000A, 0x000B,
	0x000B, 0x000C, 0x000C, 0x000D, 0x000E, 0x000E, 0x000F, 0x0010,
	0x0011, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
	0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001F, 0x0020,
	0x0021, 0x0023, 0x0024, 0x0026, 0x0028, 0x0029, 0x002B, 0x002D,
	0x002F, 0x0031, 0x0033, 0x0035, 0x0037, 0x003A, 0x003C, 0x003E,
	0x0041, 0x0044, 0x0047, 0x004A, 0x004D, 0x0050, 0x0053, 0x0057,
	0x005A, 0x005E, 0x0062, 0x0066, 0x006A, 0x006E, 0x0073, 0x0077,
	0x007C, 0x0081, 0x0086, 0x008C, 0x0092, 0x0098, 0x009E, 0x00A4,
	0x00AB, 0x00B2, 0x00B9, 0x00C0, 0x00C8, 0x00D0, 0x00D9, 0x00E1,
	0x00EA, 0x00F4, 0x00FE, 0x0108, 0x0113, 0x011E, 0x0129, 0x0135,
	0x0141, 0x014E, 0x015C, 0x016A, 0x0178, 0x0187, 0x0197, 0x01A7,
	0x01B8, 0x01CA, 0x01DC, 0x01EF, 0x0203, 0x0218, 0x022D, 0x0244,
	0x025B, 0x0273, 0x028C, 0x02A6, 0x02C1, 0x02DD, 0x02FB, 0x0319,
	0x0339, 0x035A, 0x037C, 0x03A0, 0x03C5, 0x03EB, 0x0413, 0x043D,
	0x0468, 0x0495, 0x04C4, 0x04F5, 0x0528, 0x055C, 0x0593, 0x05CC,
	0x0607, 0x0645, 0x0685, 0x06C7, 0x070C, 0x0754, 0x079F, 0x07ED,
	0x083E, 0x0892, 0x08E9, 0x0944, 0x09A3, 0x0A05, 0x0A6B, 0x0AD5,
	0x0B44, 0x0BB7, 0x0C2E, 0x0CAB, 0x0D2C, 0x0DB2, 0x0E3E, 0x0ECF,
	0x0F66, 0x1003, 0x10A6, 0x1150, 0x1201, 0x12B8, 0x1377, 0x143D,
	0x150C, 0x15E2, 0x16C1, 0x17A9, 0x189B, 0x1996, 0x1A9A, 0x1BAA,
	0x1CC3, 0x1DE9, 0x1F1A, 0x2057, 0x21A0, 0x22F7, 0x245B, 0x25CE,
	0x274F, 0x28E0, 0x2A80, 0x2C32, 0x2DF4, 0x2FC8, 0x31AF, 0x33AA,
	0x35B8, 0x37DC, 0x3A15, 0x3C65, 0x3ECD, 0x414D, 0x43E6, 0x469A,
	0x496A, 0x4C56, 0x4F60, 0x5288, 0x55D2, 0x593C, 0x5CC9, 0x607B,
	0x6452, 0x6851, 0x6C78, 0x70C9, 0x7546, 0x79F1, 0x7ECC, 0x83D8,
	0x8918, 0x8E8D, 0x9439, 0x9A20, 0xA042, 0xA6A3, 0xAD45, 0xB42B,
	0xBB57, 0xC2CC, 0xCA8D, 0xD29D, 0xDAFF, 0xE3B6, 0xECC7, 0xF633,
	0xFFFF
};
/**
 * @brief Simple integer square root, root is always rounded down
 * 
 * @param n radicand
 * @return uint16_t floor of square root
 */
uint16_t heron_sqrt(uint32_t n){
	uint32_t x0 = n / 2;
	if(x0 == 0) return n;
	uint32_t x1 = (x0 + n / x0) / 2;
	while (x1 < x0){
		x0 = x1;
		x1 = (x0 + n / x0) / 2;
	}
	return x0;
}

/**
 * @brief Filters adc values with a quadratic Savitzky-Golay filter and writes filtered values back to x and y
 * 
 * @param x new  x values of ADC and return of filtered x values
 * @param y new  y values of ADC and return of filtered y values
 */
void filter_adc(joy_val *pos){
#define ADC_HIST_LEN 5
	static uint16_t x_hist[ADC_HIST_LEN] = {0};
	static uint16_t y_hist[ADC_HIST_LEN] = {0};
	for(int i = ADC_HIST_LEN - 1; i > 0; --i){
		x_hist[i] = x_hist[i - 1];
		y_hist[i] = y_hist[i - 1];
	}
	x_hist[0] = pos->x;
	y_hist[0] = pos->y;
#if ADC_HIST_LEN == 5
	pos->x = ((-3) * x_hist[0] + 12 * x_hist[1] + 17 * x_hist[2] + 12 * x_hist[3] + (-3) * x_hist[4]) / 35;
	pos->y = ((-3) * y_hist[0] + 12 * y_hist[1] + 17 * y_hist[2] + 12 * y_hist[3] + (-3) * y_hist[4]) / 35;
#elif ADC_HIST_LEN == 9
	pos->x = ((-21) * x_hist[0] + 14 * x_hist[1] + 39 * x_hist[2] + 54 * x_hist[3] + 59 * x_hist[4] + 54 * x_hist[5] + 39 * x_hist[6] + 14 * x_hist[7] + (-21) * x_hist[8]) / 231;
	pos->y = ((-21) * y_hist[0] + 14 * y_hist[1] + 39 * y_hist[2] + 54 * y_hist[3] + 59 * y_hist[4] + 54 * y_hist[5] + 39 * y_hist[6] + 14 * y_hist[7] + (-21) * y_hist[8]) / 231;
#endif
}

/*
* Converts the desired apparent brightness to the actual brightness
* using the LUT
*
*	@param des_bright desired brightness as relative brightness*ADC-Resolution
*/
uint16_t adc_to_pwm_value(uint32_t des_bright){
	if(des_bright > 0xFFF) return 0;
	uint16_t lower = des_bright & (~0xFF);		//lower actual bound
	uint16_t lower_lut = lower / 16;			//convert to LUT index
	uint16_t upper_diff = des_bright - lower;	//interval length from des_bright to upper bound
	return ((16-upper_diff) * pwm_16[lower_lut] + upper_diff * pwm_16[lower_lut + 1]) / 16;
}

/**
 * @brief Converts the joystick ADC-values into Duty-Cycle values (with period 0xFFFF)
 */
void joystick_calc(joy_val *pos, light_val *light){
	filter_adc(pos);

	//center coordinates
	int16_t x = pos->x - ADC_OFFSET;
	int16_t y = pos->y - ADC_OFFSET;
	int32_t xn = x;
	int32_t yn = y;

	//squircle transformation
	if (x != 0 && y != 0){
		uint32_t rad = y * y / x * SQRT_SCALE / x + SQRT_SCALE;
		uint16_t sqrt_temp = heron_sqrt(rad);
		int32_t second_term = sqrt_temp * x / y;
		int32_t k = min(sqrt_temp, second_term < 0 ? -second_term: second_term);
		xn = x * k / SQRT_SCALE2 + ADC_OFFSET;
		yn = y * k / SQRT_SCALE2 + ADC_OFFSET;
	}

	// limit values to range of adc before converting to dc values
	xn = xn < 0 ? 0 : xn;
	xn = xn > 0xFFF ? 0xFFF : xn;
	yn = yn < 0 ? 0 : yn;
	yn = yn > 0xFFF ? 0xFFF : yn;
	light->x = adc_to_pwm_value(xn);
	light->y = adc_to_pwm_value(yn);
}
