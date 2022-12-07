#include "main.h"

#define PC4 0b00010000
#define PF3 0b00001000
#define PD3 0b00001000
#define PB5 0b00100000
//#define ADC0_BASE 0x40038000UL

#define GPIO_PIN_LED_BLUE PC4
#define GPIO_PIN_LED_RED PF3
#define GPIO_PIN_JOY_X PB5
#define GPIO_PIN_JOY_Y PD3

volatile uint8_t ADC0_flag = 0;

// ADC Register Struct pdf: ADC p.28 ff.
// hier (ADC)Register Map erweitert (page 819)
// ADC_PSSI & ADC_CTL groesseres OFFSET->uint64_t
typedef struct
{
	uint32_t ADC_ACTSS;
	uint32_t ADC_RIS;
	uint64_t ADC_PSSI;
	uint64_t ADC_CTL;
	// TODO:	-	Datenstruktur erweitern um auf SSFIFO0 zugreifen zu kÃ¶nnen
	uint32_t ADC_SSFIFO0 = ADC0_SSFIFO0_R;
} adc_t;

//assignment-task_start
joy_val pos;
//assignment-task-end

void pwm_init(void)
{
		SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0 + SYSCTL_RCGCPWM_R1;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC + SYSCTL_RCGC2_GPIOF;
	for (int i = 0; i < 4; ++i){
		__ASM("NOP");
	}

	//PWM0_3 for LEDB
	GPIO_PORTC_DIR_R |= GPIO_PIN_LED_BLUE; //Direction Output Register mit eins setzen
	GPIO_PORTC_DEN_R |= GPIO_PIN_LED_BLUE; //Digital Enable Register eins setzen
	GPIO_PORTC_AFSEL_R |= GPIO_PIN_LED_BLUE; // alternative Function enable  eins setzen
	GPIO_PORTC_PCTL_R |= GPIO_PCTL_PC4_M0PWM6; //hier festlegen welche Funktion pin -> PWM
	
					//3, weil der PWM 6 u 7 moduliert
	PWM0_3_CTL_R = 0; //PWM disablen  
	PWM0_3_GENA_R = PWM_0_GENA_ACTCMPAD_ONE + PWM_0_GENA_ACTLOAD_ZERO; //PWM Signal High, Action bei Ctr=0
	PWM0_3_LOAD_R = PWM_3_LOAD_LOAD_M; //Counter maximum
	PWM0_3_CMPA_R = PWM_3_CMPA_COMPA_S; //Counter start
	PWM0_3_CTL_R |= PWM_3_CTL_ENABLE; //Enable PWM
	
	//PWM1_3 for LEDR
	GPIO_PORTF_DIR_R |= GPIO_PIN_LED_RED;
	GPIO_PORTF_DEN_R |= GPIO_PIN_LED_RED;
	GPIO_PORTF_AFSEL_R |= GPIO_PIN_LED_RED;
	GPIO_PORTF_PCTL_R |= GPIO_PCTL_PF3_M1PWM7;
	
	PWM1_3_CTL_R = 0;
	PWM1_3_GENB_R = PWM_0_GENB_ACTCMPAD_ONE + PWM_0_GENB_ACTLOAD_ZERO;
	PWM1_3_LOAD_R = PWM_3_LOAD_LOAD_M;
	PWM1_3_CMPA_R = PWM_3_CMPA_COMPA_S;
	PWM1_3_CTL_R |= PWM_3_CTL_ENABLE;
	
}

void gptm_init(void)
{
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;
	//Ensure timer is disabled
	TIMER0_CTL_R = 0;
	//Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000

}

void adc_init(adc_t *adc)
{
	SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;
	//Enable clock for GPIO
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1 | SYSCTL_RCGCGPIO_R3; 
	//Direction register
	GPIO_PORTB_DIR_R &= ~GPIO_PIN_JOY_X;
	GPIO_PORTD_DIR_R &= ~GPIO_PIN_JOY_Y;
	//Set GPIO AFSEL bits for ADC input pins
	GPIO_PORTB_AFSEL_R |= GPIO_PIN_JOY_X;
	GPIO_PORTD_AFSEL_R |= GPIO_PIN_JOY_Y;
	//Configure AINx signals
	GPIO_PORTB_DEN_R &=  ~GPIO_PIN_JOY_X;
	GPIO_PORTD_DEN_R &=  ~GPIO_PIN_JOY_Y;
	//Disable analog isolation circuit 
	GPIO_PORTB_AMSEL_R |= GPIO_PIN_JOY_X;
	GPIO_PORTD_AMSEL_R |= GPIO_PIN_JOY_Y;

	adc->ADC_ACTSS &= ~ADC_ACTSS_ASEN0;


}

void ADC0_Handler(void)
{
	ADC0_flag = 1;
}

int main(void)
{
	adc_init();
	pwm_init();
	gptm_init();

	//PWM aktivieren für Port PC4 (blaue LED)
	PWM0_ENABLE_R |= PWM_ENABLE_PWM6EN;
	//PWM aktivieren für Port PF3 (Rote LED)
	PWM1_ENABLE_R |= PWM_ENABLE_PWM7EN;

	while (1)
	{
		if (ADC0_flag == 1) {
			//TODO: Werte in joystick_calc schreiben
			joystick_calc();
			ADC0_flag = 0;
		}
	}
}
