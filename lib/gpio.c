#include "gpio.h"


#define UL_REGISTER_UNSET(X, Y) (*((volatile unsigned long *)(X)) &= ~(Y))
#define UL_REGISTER_SET(X, Y) (*((volatile unsigned long *)(X)) |= (Y))
#define UL_REGISTER_SET_VALUE(X, Y) (*((volatile unsigned long *)(X)) = (Y))
#define UL_REGISTER_GET(X) (*((volatile unsigned long *)(X)))

inline void gpio_set( GPIO_Port port, uint8_t pin, uint8_t value ){
	if(value)
		UL_REGISTER_SET(port|0x3FCL, 1 << pin);	
	else
		UL_REGISTER_UNSET(port|0x3FCL	, 1 << pin);	
}

inline void gpio_get( GPIO_Port port, uint8_t pin, uint8_t* value ){
	*value = (UL_REGISTER_GET(port|0x3FCL) & (1 << pin)) != 0;
}


void gpio_init( GPIO_Port port, uint8_t pin ){
	SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOA + SYSCTL_RCGC2_GPIOB + SYSCTL_RCGC2_GPIOC + SYSCTL_RCGC2_GPIOD + SYSCTL_RCGC2_GPIOE + SYSCTL_RCGC2_GPIOF;
	delay_us(1);
	//SYSCTL_GPIOHBCTL_R = SYSCTL_GPIOHBCTL_PORTA + SYSCTL_GPIOHBCTL_PORTB + SYSCTL_GPIOHBCTL_PORTC + SYSCTL_GPIOHBCTL_PORTD + SYSCTL_GPIOHBCTL_PORTE + SYSCTL_GPIOHBCTL_PORTF;
	delay_us(1);
	
	if((port == PORTF && pin == 0) || (port == PORTD && pin == 7)){
		UL_REGISTER_SET_VALUE(port|0x520, 0x4C4F434B);		// Unlock
		UL_REGISTER_SET(port|0x524, 1 << pin);								// Commit Register
	}
	UL_REGISTER_UNSET(port|0x400UL, 1 << pin);					// Direction 0 -> input
	UL_REGISTER_UNSET(port|0x528UL, 1 << pin);					// Analog Function 0 -> disabled
	UL_REGISTER_SET(port|0x51CUL, 1 << pin);         		// Digital IO 1 -> enabled
	UL_REGISTER_UNSET(port|0x420UL, 1 << pin);					// Alternate Function 0 -> disabled
	UL_REGISTER_UNSET(port|0x510UL, 1 << pin);					// Pull up 0 -> disabled
	
}

void gpio_mode( GPIO_Port port, uint8_t pin, GPIO_Mode mode ){
	switch(mode){
		case INPUT:
			UL_REGISTER_UNSET(port|0x400UL, 1 << pin);				// Direction 0 -> input
			UL_REGISTER_UNSET(port|0x528UL, 1 << pin);				// Analog Function 0 -> disabled
			UL_REGISTER_UNSET(port|0x420UL, 1 << pin);				// Alternate Function 0 -> disabled
			UL_REGISTER_UNSET(port|0x52CUL, (15 << (4*pin)));
		  UL_REGISTER_UNSET(port|0x510UL, 1 << pin);				// Pull up 0 -> disabled
			UL_REGISTER_SET(port|0x51CUL, 1 << pin);         	// Digital IO 1 -> enabled
		
			break;
		case INPUT_PULLUP:
			UL_REGISTER_UNSET(port|0x400UL, 1 << pin);				// Direction 0 -> input
			UL_REGISTER_UNSET(port|0x528UL, 1 << pin);				// Analog Function 0 -> disabled
			UL_REGISTER_UNSET(port|0x420UL, 1 << pin);				// Alternate Function 0 -> disabled
			UL_REGISTER_UNSET(port|0x52CUL, (15 << (4*pin)));
			UL_REGISTER_SET(port|0x510UL, 1 << pin);					// Pull up 1 -> enabled
			UL_REGISTER_SET(port|0x51CUL, 1 << pin);         	// Digital IO 1 -> enabled
		
			break;
		case OUTPUT:
			UL_REGISTER_SET(port|0x400UL, 1 << pin);					// Direction 0 -> input
			UL_REGISTER_UNSET(port|0x528UL, 1 << pin);				// Analog Function 0 -> disabled
			UL_REGISTER_UNSET(port|0x420UL, 1 << pin);				// Alternate Function 0 -> disabled
			UL_REGISTER_UNSET(port|0x52CUL, (15 << (4*pin)));
			UL_REGISTER_UNSET(port|0x510UL, 1 << pin);				// Pull up 0 -> disabled
			UL_REGISTER_SET(port|0x51CUL, 1 << pin);         	// Digital IO 1 -> enabled
		
			break;
		case ALTERNATE:
			UL_REGISTER_SET(port|0x400UL, 1 << pin);					// Direction 0 -> input
			UL_REGISTER_UNSET(port|0x528UL, 1 << pin);				// Analog Function 0 -> disabled
			UL_REGISTER_UNSET(port|0x510UL, 1 << pin);				// Pull up 0 -> disabled
			UL_REGISTER_SET(port|0x51CUL, 1 << pin);         	// Digital IO 1 -> enabled
			UL_REGISTER_SET(port|0x420UL, 1 << pin);					// Alternate Function 1 -> enabled
		
			break;
		case ANALOG:
			UL_REGISTER_SET(port|0x400UL, 1 << pin);					// Direction 0 -> input
			UL_REGISTER_SET(port|0x528UL, 1 << pin);					// Analog Function 1 -> enabled
			UL_REGISTER_UNSET(port|0x510UL, 1 << pin);				// Pull up 0 -> disabled
			UL_REGISTER_UNSET(port|0x51CUL, 1 << pin);        // Digital IO 0 -> disabled
			UL_REGISTER_UNSET(port|0x420UL, 1 << pin);				// Alternate Function 0 -> disabled
		
			break;
	}
}

void gpio_alternate(GPIO_Port port, uint8_t pin, uint8_t mode){
	UL_REGISTER_SET(port|0x52CUL, (mode << (4*pin)));
}

void gpio_pullup(GPIO_Port port, uint8_t pin, uint8_t enable){
	if(enable)
		UL_REGISTER_SET(port|0x510UL, 1 << pin);
	else
		UL_REGISTER_UNSET(port|0x510UL, 1 << pin);
}

