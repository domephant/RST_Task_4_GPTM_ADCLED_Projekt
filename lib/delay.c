/**
	* Project           : TM4C123GH6PM Timer controlled delays
	* Program name      : delay.h, delay.c
	* Author            : Daniel Laube <d.laube@ostfalia.de>
	* Date created      : 20190104
	* Purpose           : Active waiting delay with timers to improve accuracy
	* Revision History  :
	*
	*		See delay.h
	*/

#include "delay.h"

static uint8_t portf_configured = 0;

void delay_us(uint32_t time){
	SYSCTL_RCGCTIMER_R |= 0x01;

	TIMER0_CTL_R = 0; 
	TIMER0_CFG_R = 0x04;
	TIMER0_TAMR_R = 0x02;
	
	if(!portf_configured)
	{
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		GPIO_PORTF_DIR_R |= (1<<2);
		GPIO_PORTF_DEN_R |= (1<<2);
		portf_configured = 1;
	}

	// 80MHz Clock -> 80 clocks for 1us
	TIMER0_TAILR_R = 80 - 1;
	TIMER0_ICR_R = 0x1;
	TIMER0_CTL_R |= 0x01;

	// Loop time times over the counter to get time us
	for(int i = 0; i < time; i++){	
		// Wait until timer stops
		while ((TIMER0_RIS_R & 0x1) == 0);
		#ifdef BEEP
		GPIO_PORTF_DATA_R ^= 0x04; //BEEP
		#endif
		// Reset timer
		TIMER0_ICR_R = 0x1;
	}
}

void delay_ms(uint32_t time){
	SYSCTL_RCGCTIMER_R |= 0x01;

	TIMER0_CTL_R = 0;
	TIMER0_CFG_R = 0x00;
	TIMER0_TAMR_R = 0x02;
	
	if(!portf_configured)
	{
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		GPIO_PORTF_DIR_R |= 0x04;
		GPIO_PORTF_DEN_R |= 0x04;
		portf_configured = 1;
	}

	// 80MHz Clock -> 80000 clocks for 1ms
	TIMER0_TAILR_R = 80000 - 1; 	
	TIMER0_ICR_R = 0x1;
	TIMER0_CTL_R |= 0x01;

	// Loop time times over the counter to get time ms
	uint8_t cnt = 0;
	for(int i = 0; i < time; i++){ 
		// Wait until timer stops
		while ((TIMER0_RIS_R & 0x1) == 0);
		#ifdef BEEP
		GPIO_PORTF_DATA_R ^= 0x04; //BEEP
		#endif
		
		// Reset timer
		TIMER0_ICR_R = 0x1;
	}

	// PWM1_CTL_R &= ~PWM_1_CTL_ENABLE;
	// PWM1_ENABLE_R &= ~PWM_ENABLE_PWM1EN;
}
