#include "pwm.h"

#define pwm_cfg_set_load 		(cfg->period)
#define pwm_cfg_set_dc			(cfg->period - cfg->duty_cycle)
#define pwm_cfg_set_phase_start	(cfg->phase_shift % cfg->period)
#define pwm_cfg_set_phase_end	((cfg->phase_shift + cfg->duty_cycle) % cfg->period)

#define pwm_submod_addr ((pwm_submod_hw*)((pwm_submod_hw*)cfg->base_ptr +cfg->channel / 2 + 1))

struct channel_io pwm_channel_to_port [2][8] = {
    {   //PWM0
        {PORTB, 6, 4},
        {PORTB, 7, 4},
        {PORTB, 4, 4},
        {PORTB, 5, 4},
        {PORTE, 4, 4},
        {PORTE, 5, 4},
        {PORTC, 4, 4},  //kann auch {PORTD, 0, 4} sein
        {PORTC, 5, 4}   //kann auch {PORTD, 1, 4} sein
    },
    {   //PWM1
        {PORTD, 0, 5},
        {PORTD, 1, 5},
        {PORTA, 6, 5},  //kann auch {PORTE, 4, 5} sein
        {PORTA, 7, 5},  //kann auch {PORTE, 5, 5} sein
        {PORTF, 0, 5},  //Locked
        {PORTF, 1, 5},
        {PORTF, 2, 5},
        {PORTF, 3, 5}
    }
};

/**
*	The possible actions for PWM Generators
*/
enum pwm_gen_actions{
    ACT_ZERO = 0,		//Do nothing
    ACT_INVERT = 1,		//Invert the signal
    ACT_LOW = 2,		//Set signal Low
    ACT_HIGH = 3		//set signal High
};


void pwm_load_std_cfg(pwm_channel_cfg *cfg){
	*cfg = (pwm_channel_cfg){
		(pwm_mod_hw*)PWM0,						//Base Module
		0,							//Channel
		PWMMODE_BASIC_DOWN,			//Mode
		0xFFFF,						//Period
		0x8000,						//Duty Cycle
		0,							//Run in Debug
		0x0							//Phase Shift (in this mode irrelevant)
	};
}

void pwm_init(pwm_channel_cfg *cfg){
    pwm_mod_hw *hw = cfg->base_ptr;
	pwm_submod_hw *sub = pwm_submod_addr;
    SYSCTL->RCGCPWM |= (hw == (pwm_mod_hw*)PWM0_BASE) ? 0x1 : 0x2;
	for (int i = 0; i < 4; ++i){
		__ASM("NOP");
	}

	SYSCTL->RCC &= ~(0x3 << 17);	//PWMDIV = 0
	SYSCTL->RCC |= 1 << 20;			//USE PWM DIV
	//unlock PF0 if needed, initialize GPIO pin
	struct channel_io channel_pin = pwm_channel_to_port[hw == (pwm_mod_hw*)PWM1_BASE][cfg->channel];
	if (channel_pin.port == PORTF && channel_pin.pin == 0){
		((GPIOA_Type*)channel_pin.port)->LOCK = GPIO_LOCK_KEY;
		((GPIOA_Type*)channel_pin.port)->CR |= (1 << cfg->channel);
	}
	gpio_init(channel_pin.port, channel_pin.pin);
	gpio_mode(channel_pin.port, channel_pin.pin, ALTERNATE);
	gpio_alternate(channel_pin.port, channel_pin.pin, channel_pin.mux);

	//PWM Clock Div?
	switch(cfg->mode){
		case PWMMODE_BASIC_DOWN:
			sub->CTL.REG = 0x280;	//all updates immediate, no debug run, down mode, module off
			break;
		case PWMMODE_BASIC_SHIFT:
			sub->CTL.REG = 0;	//all updates immediate, no debug run, down mode, module off
			break;
	}

	pwm_set_dc(cfg);

	sub->CTL.B.DEBUG = cfg->debug_run;
	sub->CTL.B.ENABLE = 1;
}


void pwm_set_dc(pwm_channel_cfg *cfg){
	pwm_submod_hw *sub = pwm_submod_addr;
	//check to completely turn off pwm signal
	if (cfg->duty_cycle == 0){
		cfg->base_ptr->ENABLE.REG &= ~(1<<cfg->channel);
	} else {
		cfg->base_ptr->ENABLE.REG |= (1<<cfg->channel);
	}
	switch(cfg->mode){
		case PWMMODE_BASIC_DOWN:
			sub->LOAD.B.LOAD = pwm_cfg_set_load;
			sub->CMPA.B.COMPA = pwm_cfg_set_dc;
			
			if (cfg->channel %2 == 0){	//Channel A
				sub->GENA.B.ACTZERO = ACT_HIGH;
				sub->GENA.B.ACTCMPAD = ACT_LOW;
			} else {
				sub->GENB.B.ACTZERO = ACT_HIGH;
				sub->GENB.B.ACTCMPAD = ACT_LOW;
			}
			break;
		case PWMMODE_BASIC_SHIFT:
			sub->LOAD.B.LOAD = pwm_cfg_set_load;
			sub->CMPA.B.COMPA = pwm_cfg_set_phase_start;
			sub->CMPB.B.COMPB = pwm_cfg_set_phase_end;
			if (cfg->channel %2 == 0){	//Channel A
				sub->GENA.B.ACTCMPAD = ACT_LOW;
				sub->GENA.B.ACTCMPBD = ACT_HIGH;
			} else {
				sub->GENB.B.ACTCMPAD = ACT_LOW;
				sub->GENB.B.ACTCMPBD = ACT_HIGH;
			}
			break;
	}
}
