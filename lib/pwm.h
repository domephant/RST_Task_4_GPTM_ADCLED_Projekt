#ifndef __PWM_DRIVER_HEADER__
#define __PWM_DRIVER_HEADER__

#include "TM4C123.h"
#include "gpio.h"

/**
* type that is used for every field that is tied to the period length (ie duty cycle)
*/
typedef uint16_t pwm_period_type;

/**
* Type that is used for every field that describes the channel number
*/
typedef uint8_t pwm_channel_type;

/**
* 	Struct for bundling GPIO information
*	used to map a PWM signal to a GPIO pin
*/
struct channel_io {
    GPIO_Port port;		//Base address of Port
    uint8_t pin;		//Pin number
    uint8_t mux;		//mux that is entered in PCTL
};

/**
* available PWM Channel Presets
*/
enum pwm_channel_mode{
	/*
	*	Counts down, reset is HIGH, 
	* falling edge is determined by dc
	* immediate update of everything
	*/
    PWMMODE_BASIC_DOWN,
	/*
	*	counts down, phase shift sets high
	*	falling edge set by dc
	*	can go over period boundary
	*	immediate update of everything
	*/
    PWMMODE_BASIC_SHIFT,
};

/**
* Register Map for PWM Submodules
*/
typedef struct{
    union{
        uint32_t REG;       //Reset: 0x0000.0000
        struct{
            uint32_t ENABLE     : 1; //0: PWM generation block disabled and unclocked, 1: Block enabled and clocked
            uint32_t MODE       : 1; //0: Dount-Down mode, 1: Count-Up/Down mode
            uint32_t DEBUG      : 1; //0: when in debug mode, counter stops at 0, 1: counter runs in debug mode

            /*Update Register Values:
            *   the correspondings registers are updated:
            *   0: locally synchronized
            *   1: globally synchronized */
            uint32_t LOADUPD    : 1; //Register: PWMnLOAD
            uint32_t CMPAUPD    : 1; //Register: PWMnCMPA
            uint32_t CMPBUPD    : 1; //Register: PWMnCMPB

            /*Update Register values:
            *   the values of the corresponding Registers are updated:
            *   0: immediately
            *   1: reserved
            *   2: locally synchronized
            *   3: globally synchronized */
            uint32_t GENAUPD    : 2; //Register: PWMnGENA
            uint32_t GENBUPD    : 2; //Register: PWMnGENB
            uint32_t DBCTLUPD   : 2; //Register: PWMnDBCTL
            uint32_t DBRISEUPD  : 2; //Register: PWMnDBRISE
            uint32_t DBFALLUPD  : 2; //Register: PWMnDBFALL

            uint32_t FLTSRC     : 1; //0: Fault source is set by Fault0 input
                                     //1:PWMnFLTSRC0 und PWMnFLTSRC1 define source
            uint32_t MINFLTPER  : 1; //0: Fault input deassertion is unaffected
                                     //1: one-shot counter active and extends fault period to a minimum
            uint32_t LATCH      : 1; //0: Fault Condition is in effect as long as generating source is asserting
                                     //1: Fault Condition only clears through INTFAULTn in ISC
        } B;
    } CTL;       //0x40, Control
    union{
        uint32_t REG;
        struct{
            uint32_t INTCNTZERO : 1; //enables interrupt when counter is zero
            uint32_t INTCNTLOAD : 1; //enables interrupt when counter matches value in PWMnLOAD
            uint32_t INTCMPAU : 1;   //enables interrupt when counter matches CMPA value while counting up
            uint32_t INTCMPAD : 1;   //enables interrupt when counter matches CMPA value while counting down
            uint32_t INTCMPBU : 1;   //enables interrupt when counter matches CMPB value while counting up
            uint32_t INTCMPBD : 1;   //enables interrupt when counter matches CMPB value while counting down
            uint32_t : 2;
            uint32_t TRCNTZERO : 1;  //enables adc trigger when counter is zero
            uint32_t TRCNTLOAD : 1;  //enables adc trigger when counter matches value in PWMnLOAD
            uint32_t TRCMPAU : 1;    //enables adc trigger when counter matches CMPA value while counting up
            uint32_t TRCMPAD : 1;    //enables adc trigger when counter matches CMPA value while counting down
            uint32_t TRCMPBU : 1;    //enables adc trigger when counter matches CMPB value while counting up
            uint32_t TRCMPBD : 1;    //enables adc trigger when counter matches CMPB value while counting down
        } B;
    } INTEN;     //0x44, Interrupt and Trigger Enable
    union{
        uint32_t REG;
        struct{
            uint32_t INTCNTZERO : 1;//the counter is zero
            uint32_t INTCNTLOAD : 1;//the counter has matched the value in PWMnLOAD
            uint32_t INTCMPAU : 1;  //the counter has matched the CMPA value while counting up
            uint32_t INTCMPAD : 1;  //the counter has matched the CMPA value while counting down
            uint32_t INTCMPBU : 1;  //the counter has matched the CMPB value while counting up
            uint32_t INTCMPBD : 1;  //the counter has matched the CMPB value while counting down
        } B;
    } RIS;       //0x48, Raw Interrupt Status
    union{
        uint32_t REG;
        struct{
            uint32_t INTCNTZERO : 1;//clears the corresponding interrupt in PWMnRIS
            uint32_t INTCNTLOAD : 1;//clears the corresponding interrupt in PWMnRIS
            uint32_t INTCMPAU : 1;  //clears the corresponding interrupt in PWMnRIS
            uint32_t INTCMPAD : 1;  //clears the corresponding interrupt in PWMnRIS
            uint32_t INTCMPBU : 1;  //clears the corresponding interrupt in PWMnRIS
            uint32_t INTCMPBD : 1;  //clears the corresponding interrupt in PWMnRIS
        } B;
    } ISC;       //0x4C, Interrupt Status and Clear
    union{
        uint32_t REG;
        struct{
            uint32_t LOAD : 16;     //the counter load value
        } B;
    } LOAD;      //0x50, Load
    union{
        uint32_t REG;
        struct{
            uint32_t COUNT : 16;    //the current value of the counter
        } B;
    } COUNT;     //0x54, Counter
    union{
        uint32_t REG;
        struct{
            uint32_t COMPA : 16;    //Comparator A value
        } B;
    } CMPA;      //0x58, Compare A
    union{
        uint32_t REG;
        struct{
            uint32_t COMPB : 16;    //Comparator B value
        } B;
    } CMPB;      //0x5C, Compare B
    union{
        uint32_t REG;
        struct{
            /* The possible actions are as follows:
            *   0: Do nothing
            *   1: Invert pwmA
            *   2: Drive pwmA low
            *   3: Drive pwmA high */
            uint32_t ACTZERO : 2;   //counter takes specified action when reaching zero
            uint32_t ACTLOAD : 2;   //counter takes specified action when reaching LOAD
            uint32_t ACTCMPAU : 2;  //counter takes specified action when reaching COMPA while counting up
            uint32_t ACTCMPAD : 2;  //counter takes specified action when reaching COMPA while counting down
            uint32_t ACTCMPBU : 2;  //counter takes specified action when reaching COMPB while counting up
            uint32_t ACTCMPBD : 2;  //counter takes specified action when reaching COMPB while counting down
        } B;
    } GENA;      //0x60, Generator A Control
    union{
        uint32_t REG;
        struct{
            /* The possible actions are as follows:
            *   0: Do nothing
            *   1: Invert pwmB
            *   2: Drive pwmB low
            *   3: Drive pwmB high */
            uint32_t ACTZERO : 2;   //counter takes specified action when reaching zero
            uint32_t ACTLOAD : 2;   //counter takes specified action when reaching LOAD
            uint32_t ACTCMPAU : 2;  //counter takes specified action when reaching COMPA while counting up
            uint32_t ACTCMPAD : 2;  //counter takes specified action when reaching COMPA while counting down
            uint32_t ACTCMPBU : 2;  //counter takes specified action when reaching COMPB while counting up
            uint32_t ACTCMPBD : 2;  //counter takes specified action when reaching COMPB while counting down
        } B;
    } GENB;      //0x64, Generator B Control
    union{
        uint32_t REG;
        struct{
            uint32_t ENABLE : 1;    //enables Dead-Band generation and modifies pwmA' and pwmB'
        } B;
    } DBCTL;     //0x68, Dead-Band-Control
    union{
        uint32_t REG;
        struct{
            uint32_t RISEDELAY : 11;//number of clock cycles to delay rising edge of pwmA' after RE of pwmA
        } B;
    } DBRISE;    //0x6C, Dead-Band Rising-Edge Delay
    union{
        uint32_t REG;
        struct{
            uint32_t FALLDELAY : 11;//number of clock cycles to delay falling edge of pwmA' after Rising edge of pwmA
        } B;
    } DBFALL;    //0x70, Dead-Band Falling-Edge Delay
    union{
        uint32_t REG;
        struct{
            uint32_t FAULT0 : 1;    //enables Fault0 to generate a fault condition
            uint32_t FAULT1 : 1;    //enables Fault1 to generate a fault condition
        } B;
    } FLTSRC0;   //0x74, Fault Source 0
    union{
        uint32_t REG;
        struct{
            uint32_t DCMP0 : 1;     //enables the digital comparator 0 to generate a fault condition
            uint32_t DCMP1 : 1;     //enables the digital comparator 1 to generate a fault condition
            uint32_t DCMP2 : 1;     //enables the digital comparator 2 to generate a fault condition
            uint32_t DCMP3 : 1;     //enables the digital comparator 3 to generate a fault condition
            uint32_t DCMP4 : 1;     //enables the digital comparator 4 to generate a fault condition
            uint32_t DCMP5 : 1;     //enables the digital comparator 5 to generate a fault condition
            uint32_t DCMP6 : 1;     //enables the digital comparator 6 to generate a fault condition
            uint32_t DCMP7 : 1;     //enables the digital comparator 7 to generate a fault condition
        } B;
    } FLTSRC1;   //0x78, Fault Source 1
    union{
        uint32_t REG;
        struct{
            uint32_t MFP : 16;      //number of PWM clocks by which a fault condition is extended if enabled by CTL
        } B;
    } MINFLTPER; //0x7C, Minimum Fault Period
} pwm_submod_hw;    //Size: 0x40

/**
* Register Map for additional Fault registers
*/
typedef struct{
    union{
        uint32_t REG;
        struct{
            uint32_t FAULT0 : 1;    //an error is indicated if Fault0 signal is 0: Low or 1: High
            uint32_t FAULT1 : 1;    //an error is indicated if Fault1 signal is 0: Low or 1: High
        } B;
    } FLT_SEN;                  //Fault Pin Logic Sense
    union{
        uint32_t REG;
        struct{
            uint32_t FAULT0 : 1;    //see pg. 1298
            uint32_t FAULT1 : 1;    //see pg. 1298
        } B;
    } FLTSTAT0;                 //Fault Status 0 (Fault Input)
    union{
        uint32_t REG;
        struct{
            uint32_t DCMP0 : 1;     //see pg. 1300f
            uint32_t DCMP1 : 1;     //see pg. 1300f
            uint32_t DCMP2 : 1;     //see pg. 1300f
            uint32_t DCMP3 : 1;     //see pg. 1300f
            uint32_t DCMP4 : 1;     //see pg. 1300f
            uint32_t DCMP5 : 1;     //see pg. 1300f
            uint32_t DCMP6 : 1;     //see pg. 1300f
            uint32_t DCMP7 : 1;    	//see pg. 1300f
        } B;
    } FLTSTAT1;                 //Fault Status 1 (Digital Comparator Trigger)
} pwm_fault_hw; //Size: 0x0C

/**
* Register Map for PWM Modules
*/
typedef struct{
    union{
      uint32_t REG;                 //Reset: 0x000.000
      struct {
        uint32_t GLOBALSYNC0 : 1;	//Update PWM Generator n
        uint32_t GLOBALSYNC1 : 1;   //when set to 1, queued updates are applied
        uint32_t GLOBALSYNC2 : 1;   //next time counter reaches zero
        uint32_t GLOBALSYNC3 : 1;
      } B; 
    } CTL;                      //0x00, Master Control
    union{
        uint32_t REG;               //Reset: 0x0000.0000
        struct{
            uint32_t SYNC0 : 1;     //writing 1 resets PWM Generator n Counter
            uint32_t SYNC1 : 1;     //self clears after reset
            uint32_t SYNC2 : 1;     //reading zero means reset has completed
            uint32_t SYNC3 : 1;
        } B;
    } SYNC;                     //0x04, Time Base Sync
    union{
        uint32_t REG;               //Reset: 0x0000.0000
        struct{
            uint32_t PWM0EN : 1;    //0: MnPWM0 Pin has zero value, 1: pwm0A is passed on
            uint32_t PWM1EN : 1;    //0: MnPWM1 Pin has zero value, 1: pwm0B is passed on
            uint32_t PWM2EN : 1;    //0: MnPWM2 Pin has zero value, 1: pwm1A is passed on
            uint32_t PWM3EN : 1;    //0: MnPWM3 Pin has zero value, 1: pwm1B is passed on
            uint32_t PWM4EN : 1;    //0: MnPWM4 Pin has zero value, 1: pwm2A is passed on
            uint32_t PWM5EN : 1;    //0: MnPWM5 Pin has zero value, 1: pwm2B is passed on
            uint32_t PWM6EN : 1;    //0: MnPWM6 Pin has zero value, 1: pwm3A is passed on
            uint32_t PWM7EN : 1;   	//0: MnPWM7 Pin has zero value, 1: pwm3B is passed on
        } B;
    } ENABLE;                   //0x08, Output Enable
    union{
        uint32_t REG;                 //Reset: 0x0000.0000
        union{
            uint32_t PWM0INV : 1;   //0: MnPWM0 Signal is not inverted, 1: MnPWM0 Signal is inverted
            uint32_t PWM1INV : 1;   //0: MnPWM1 Signal is not inverted, 1: MnPWM1 Signal is inverted
            uint32_t PWM2INV : 1;   //0: MnPWM2 Signal is not inverted, 1: MnPWM2 Signal is inverted
            uint32_t PWM3INV : 1;   //0: MnPWM3 Signal is not inverted, 1: MnPWM3 Signal is inverted
            uint32_t PWM4INV : 1;   //0: MnPWM4 Signal is not inverted, 1: MnPWM4 Signal is inverted
            uint32_t PWM5INV : 1;   //0: MnPWM5 Signal is not inverted, 1: MnPWM5 Signal is inverted
            uint32_t PWM6INV : 1;   //0: MnPWM6 Signal is not inverted, 1: MnPWM6 Signal is inverted
            uint32_t PWM7INV : 1;   //0: MnPWM7 Signal is not inverted, 1: MnPWM7 Signal is inverted
        } B;
    } INVERT;                   //0x0C, Output Inversion
    union{
        uint32_t REG;                 //Reset: 0x0000.0000
        struct
        {
            uint32_t FAULT0 : 1;    //0: pwm0A is passed to MnPWM0, 1: MnPWM0 is specified by PWMFAULTVAL
            uint32_t FAULT1 : 1;    //0: pwm0B is passed to MnPWM1, 1: MnPWM1 is specified by PWMFAULTVAL
            uint32_t FAULT2 : 1;    //0: pwm1A is passed to MnPWM2, 1: MnPWM2 is specified by PWMFAULTVAL
            uint32_t FAULT3 : 1;    //0: pwm1B is passed to MnPWM3, 1: MnPWM3 is specified by PWMFAULTVAL
            uint32_t FAULT4 : 1;    //0: pwm2A is passed to MnPWM4, 1: MnPWM4 is specified by PWMFAULTVAL
            uint32_t FAULT5 : 1;    //0: pwm2B is passed to MnPWM5, 1: MnPWM5 is specified by PWMFAULTVAL
            uint32_t FAULT6 : 1;    //0: pwm3A is passed to MnPWM6, 1: MnPWM6 is specified by PWMFAULTVAL
            uint32_t FAULT7 : 1;    //0: pwm3B is passed to MnPWM7, 1: MnPWM7 is specified by PWMFAULTVAL
        } B;
    } FAULT;                    //0x10, Output Fault
    union{
        uint32_t REG;                 //Reset: 0x0000.0000
        struct{
            uint32_t INTPWM0 : 1;   //enables interrupts for PWM Generator 1
            uint32_t INTPWM1 : 1;   //enables interrupts for PWM Generator 0
            uint32_t INTPWM2 : 1;   //enables interrupts for PWM Generator 2
            uint32_t INTPWM3 : 1;   //enables interrupts for PWM Generator 3
            uint32_t : 12;
            uint32_t INTFAULT0 : 1; //enables Fault Condition Interrupts for PWM Generator 0
            uint32_t INTFAULT1 : 1; //enables Fault Condition Interrupts for PWM Generator 1
        } B;
    } INTEN;                    //0x14, Interrupt Enable
    union{
        uint32_t REG;                 //Reset: 0x0000.0000
        struct{
            uint32_t INTPWM0 : 1;   //whether PWM Generator 0 block interrupt is asserted
            uint32_t INTPWM1 : 1;   //whether PWM Generator 0 block interrupt is asserted
            uint32_t INTPWM2 : 1;   //whether PWM Generator 0 block interrupt is asserted
            uint32_t INTPWM3 : 1;   //whether PWM Generator 0 block interrupt is asserted
            uint32_t : 12;
            uint32_t INTFAULT0 : 1; //whether Fault Condition for PWM Generator 0 is asserted
            uint32_t INTFAULT1 : 1; //whether Fault Condition for PWM Generator 1 is asserted
        } B;
    } RIS;                      //0x18, Raw Interrupt Status
    union{
        uint32_t REG;                 //Reset: 0x0000.0000
        struct{
            uint32_t INTPWM0 : 1;   //clear this bit and the corresponding bit in RIS be writing 1
            uint32_t INTPWM1 : 1;   //clear this bit and the corresponding bit in RIS be writing 1
            uint32_t INTPWM2 : 1;   //clear this bit and the corresponding bit in RIS be writing 1
            uint32_t INTPWM3 : 1;   //clear this bit and the corresponding bit in RIS be writing 1
            uint32_t : 12;
            uint32_t INTFAULT0 : 1; //clear only possible if corresponding PWMn.CTL Latch Bit is set
            uint32_t INTFAULT1 : 1; //clear only possible if corresponding PWMn.CTL Latch Bit is set
        } B;
    } ISC;                      //0x1C, Interrupt Status and Clear
    union{
        uint32_t REG;                 //Reset: 0x0000.0000
        struct{
            uint32_t FAULT0 : 1;    //whether Fault condition for PWM Generator 0 is asserted
            uint32_t FAULT1 : 1;    //whether Fault condition for PWM Generator 1 is asserted
        } B;
    } STATUS;                   //0x20, Status
    union{
        uint32_t REG;               //Reset: 0x0000.0000
        struct{
            uint32_t PWM0LVL : 1;      //whether MnPWM0 is driven High/Low during Fault Condition and set FAULT bit
            uint32_t PWM1LVL : 1;      //whether MnPWM1 is driven High/Low during Fault Condition and set FAULT bit
            uint32_t PWM2LVL : 1;      //whether MnPWM2 is driven High/Low during Fault Condition and set FAULT bit
            uint32_t PWM3LVL : 1;      //whether MnPWM3 is driven High/Low during Fault Condition and set FAULT bit
            uint32_t PWM4LVL : 1;      //whether MnPWM4 is driven High/Low during Fault Condition and set FAULT bit
            uint32_t PWM5LVL : 1;      //whether MnPWM5 is driven High/Low during Fault Condition and set FAULT bit
            uint32_t PWM6LVL : 1;      //whether MnPWM6 is driven High/Low during Fault Condition and set FAULT bit
            uint32_t PWM7LVL : 1;      //whether MnPWM7 is driven High/Low during Fault Condition and set FAULT bit
        } B;
    } FAULTVAL;                 //0x24, Fault Condition Value
    union{
        uint32_t REG;               //Reset: 0x0000.0000
        struct{
            /* Possible Values:
            *   0: Immediate, writes to PWMnEN in ENABLE are used by the generator immediately
            *   1: Reserved
            *   2: Locally Synchronized, writes to PWMnEN in ENABLE are used next time the counter is 0
            *   3: Globally Snychronized, writes to PWMnEN in ENABLE are used next time counter is 0 AND
            *       a synchronous update has been requested through Master Control (PWMCTL) */
            uint32_t ENUPD0 : 2;
            uint32_t ENUPD1 : 2;
            uint32_t ENUPD2 : 2;
            uint32_t ENUPD3 : 2;
            uint32_t ENUPD4 : 2;
            uint32_t ENUPD5 : 2;
            uint32_t ENUPD6 : 2;
            uint32_t ENUPD7 : 2;
        } B;
    } ENUPD;                 //0x28, Enable Update
    uint32_t RESERVED0[5];
    union{
        struct{
            pwm_submod_hw SUBMODULE_0;  //0x040
            pwm_submod_hw SUBMODULE_1;  //0x080
            pwm_submod_hw SUBMODULE_2;  //0x0C0
            pwm_submod_hw SUBMODULE_3;  //0x100
        };
        pwm_submod_hw SUBMODULE[4];
    };  //Ends on 0x140
    uint32_t RESERVED1[0x1B0];
    pwm_fault_hw PWM0FAULT; //0x800, Fault Registers for PWM0
    uint32_t RESERVED2[0x1D];
    pwm_fault_hw PWM1FAULT; //0x880, Fault Registers for PWM1
    uint32_t RESERVED3[0x1E];
    pwm_fault_hw PWM2FAULT; //0x904, Fault Registers for PWM2
    uint32_t RESERVED4[0x1D];
    pwm_fault_hw PWM3FAULT; //0x984, Fault Registers for PWM3
    uint32_t RESERVED5[0x18C];
    union{
        uint32_t REG;
        struct{
            uint32_t GCNT : 4;          //number of generators (0x5 - 0xF reserved), twice as many PWM outputs
            uint32_t FCNT : 4;          //number of Fault Inputs (0x5-0xF reserved)
            uint32_t ESYNC : 1;         //whether extended synchronization capabilities are available
            uint32_t EFAULT : 1;        //whether extended Fault capabilities are available
            uint32_t ONE : 1;           //whether one-shot modes are available
        } B;
    } PP;            //0xFC0, Peripheral Properties
} pwm_mod_hw;   //Size: 0xFC4

/**
* struct for bundling config info of a single channel
*/
typedef struct{
    pwm_mod_hw *base_ptr;			//Base Address of the PWM Module
    pwm_channel_type channel;		//Channel Number, from this the Submodule and GPIO Pin are inferred
    enum pwm_channel_mode mode;		//Mode preset
    pwm_period_type period;			//How many clock cycles in a period
    pwm_period_type duty_cycle;		//Duty Cycle
    uint8_t debug_run;				//Whether the PWM Counter continues counting if chip is stoppen in debug
    pwm_period_type phase_shift;    //only used if mode==PWMMODE_BASIC_SHIFT
} pwm_channel_cfg;

/**
*	Maps each possible channel to a GPIO Pin
*	For some channels there are other options. They have to be manually changed
*	in this struct, if the second option is preferred.
*
*	pwm_channel_to_port[PWM_MODULE][PWM_SUBMODULE]
*/
extern struct channel_io pwm_channel_to_port [2][8];

/**
* Get-function for the default values of a pwm_channel_cfg
*
* 	@param	cfg	struct that the default values are written into
*/
void pwm_load_std_cfg(pwm_channel_cfg *cfg);

/**
* Pwm initialization routine
*	- sets the system clock
*	- initializes the GPIO pin
*	- applies the given config
*
*	@param	cfg		the config containing which module, which channel, which mode etc.
*/
void pwm_init(pwm_channel_cfg *cfg);

/**
*	Updates the LOAD, CMP and GEN regsters according to the config
*	@param	cfg		the applied config
*/
void pwm_set_dc(pwm_channel_cfg *cfg);

#endif
