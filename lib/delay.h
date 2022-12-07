/**
	* Project           : TM4C123GH6PM Timer controlled delays
	* Program name      : delay.h, delay.c
	* Author            : Daniel Laube <d.laube@ostfalia.de>
	* Date created      : 20190104
	* Purpose           : Active waiting delay with timers to improve accuracy
	* Revision History  :
	*
	* Date        Author      													Ref    Comment
	* 20190104   	Daniel Laube <d.laube@ostfalia.de>    1      First implementation, delay_us and delay_ms
	*
	*/

#ifndef DELAY_H_
	#define DELAY_H_

	#include <stdint.h>
	#include "tm4c123gh6pm.h"

	/**
		* Delay microseconds routine
		* - Uses TIMER0 to actively delay program flow
		*
		* @param	time		time to wait in microseconds
		*/
	void delay_us(uint32_t time);

	/**
		* Delay milliseconds routine
		* - Uses TIMER0 to actively delay program flow
		*
		* @param	time		time to wait in milliseconds
		*/
	void delay_ms(uint32_t time);

#endif
