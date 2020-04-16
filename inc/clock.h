/**********************************************************************************************************
 * Project     : LPC4088FET208
 * Author      :
 * Version     :
 * Copyright   :
 * Description : Clock definition
 **********************************************************************************************************/

#ifndef CLOCK_H_
#define CLOCK_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define XTAL        				(12000000UL)        // Oscillator frequency
#define OSC_CLK     				(      XTAL)        // Main oscillator frequency
#define RTC_CLK     				(   32768UL)        // RTC oscillator frequency
#define IRC_OSC     				(12000000UL)        // Internal RC oscillator frequency
//#define WDT_OSC		  				(  500000UL)		  // Internal WDT oscillator frequency



extern uint32_t SystemCoreClock;      					// System Clock Frequency (Core Clock)
extern uint32_t PeripheralClock;	    				  // Peripheral Clock Frequency (Pclk)
extern uint32_t EMCClock;			        			    // EMC Clock
extern uint32_t USBClock;			        			    // USB Frequency


extern void SystemInit (void);
extern void SystemCoreClockUpdate (void);


#ifdef __cplusplus
}
#endif


#endif // !CLOCK_H_
