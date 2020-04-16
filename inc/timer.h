/**********************************************************************************************************
 * Project     : LPC4088FET208
 * Author      :
 * Version     :
 * Copyright   : 
 * Description : TIMER definition
 **********************************************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "LPC407x_8x_177x_8x.h"
#include "clock.h"
#include <stdint.h>


#define CEN								0
#define CRST								1
#define c_CPU_CLK							120000000
extern uint32_t SystemCoreClock;
int _v_i = 0;



class TIMER{
public:
	/******************************************************************************
	\brief		Initialize Timer1 peripheral
	\param		none
	\note		Internal timer: 	DMA trasnfer timing
	******************************************************************************/
	inline void Timer1_Init(void){


		LPC_SC->PCONP |= (1 << 2);							//Timer Stromversorgung

		LPC_TIM1->CTCR = (0 << 0);							//Timer Mode: every rising PCLK edge
		LPC_TIM1->PR = 0;								//The 32-bit Prescale register specifies the maximum value for the Prescale Counter.
		LPC_TIM1->TCR = (1 << 1);							//reset aktivieren
		LPC_TIM1->MR1 = 15;								//Timer counter match value. 8MHZ=15-1
		LPC_TIM1->MCR = (1 << 4);							//TC will be reset if MR1 matches it.
		LPC_TIM1->TCR = (1 << 1);							//reset aktivieren
	}

	/******************************************************************************
	\brief		Initialize Timer 2 MR0
	\param		_v_Frequenz		Timer frequency
	\note
	******************************************************************************/
	inline void Timer2_Init(uint32_t _v_Frequenz){
		uint32_t _v_temp = (SystemCoreClock / _v_Frequenz / 2) - 1;

	    LPC_SC->PCONP |= (1 << 22);							// Power & Clock: Timer 2
	    LPC_IOCON->P0_6 = (3 << 0);							// Timer2 10MHz Output (not active)
	    LPC_GPIO0->DIR = (1 << 6);							// Pin is defined as Outpu
	    LPC_TIM2->CTCR = (0 << 0);							// Count Control Register: Timer Mode
	    LPC_TIM2->PR = 0;								// Prescaler Vale
	    LPC_TIM2->TCR = (1 << 1);							// Timer Control Register: Timer Counter and Prescale Counter synchronously reset
	    LPC_TIM2->TC = 1;								// Timer Counter
	    LPC_TIM2->MR0 = _v_temp;							// Match Register: Maximum Value
	    LPC_TIM2->MCR = (1 << 1)							// Macht Control Register: Reset and Interrupt if TC matches MR
						 |(1 << 10);				//
	    LPC_TIM2->EMR = 0x31;							// External Match Register: No Pin toggling (0x31 = Value for Pin toggling)
	    LPC_TIM2->TCR = (1 << 1);							// Timer Counter and Prescale Counter synchronously reset
	}

	/******************************************************************************
	\brief		Initialize Timer 3 MR2
	\param		_v_Frequenz		Timer frequency
	\note
	******************************************************************************/
	inline uint32_t Timer3_Init(uint32_t _v_Frequenz){
		uint32_t _v_temp = (SystemCoreClock / _v_Frequenz / 2) - 1;

	    LPC_SC->PCONP |= (1 << 23);							// Power & Clock: Timer 3
	    LPC_IOCON->P5_2 = (0x03 << 0);						//Timer3 107kHz Output
	    LPC_GPIO5->DIR |= (1 << 2);							//Pin defined as Output
	    LPC_TIM3->CTCR = (0 << 0);							// Count Control Register: Timer Mode
	    LPC_TIM3->PR = 0;								// Prescale Value
	    LPC_TIM3->TCR = (1 << 1);							// Timer Control Register: Timer Counter and Prescale Counter synchronously reset
	    LPC_TIM3->MR2 = _v_temp;							// Match register Value: Settings for 122kHz Pulse
	    LPC_TIM3->MCR = (1 << 7);
	    LPC_TIM3->EMR = 0x304;							// External Match register: Pin toggling enabled
	    LPC_TIM3->TCR = (1 << 1);							// Timer Counter and prescale Counter synchronously reset

	    return _v_Frequenz;
	}

	/******************************************************************************
	\brief		Initialize Timer 3 MR2
	\param		_v_Frequenz			Timer frequency
	\param		_v_Anzahl_Burst		number of bursts
	\note		Timer 3 must be initialized first
	******************************************************************************/
	inline void Timer3_Burst(uint32_t _v_Anzahl_Burst, uint32_t _v_Frequenz){

		uint32_t _v_warteZeit = ((_v_Anzahl_Burst * c_CPU_CLK / _v_Frequenz) - 1) / 12;

		LPC_TIM3->TCR |= (1 << CRST);
		LPC_TIM3->TCR &= ~(1 << CRST);
		LPC_TIM3->TCR |= (1 << CEN);

		for(uint32_t i = 0; i < _v_warteZeit; i++){
			__NOP();
		}

		LPC_TIM3->TCR &= ~(1 << CEN);
	}

};



#endif /* !TIMER_H_ */
