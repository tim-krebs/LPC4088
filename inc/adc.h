/**********************************************************************************************************
 * Project     : LPC4088FET208
 * Author      :
 * Version     :
 * Copyright   : 
 * Description : DAC definition
 **********************************************************************************************************/

#ifndef DAC_H_
#define DAC_H_

#include "LPC407x_8x_177x_8x.h"
#include <stdint.h>
#include <stdio.h>
#include "mathfunction.h"

#define DAC_VALUE(n)        						((uint32_t) ((n & 0x3FF) << 6))
#define DAC_BIAS_EN         						((uint32_t) (1 << 16))

MATH math;

class DAC{
public:
	/******************************************************************************
	\brief		DAC Peripheral initialization
				   - Maximum	current is 700 uA
	               - Value to AOUT is 0
	\param		pDAC	Pointer to DAC Peripheral
	\note		none
	******************************************************************************/
	inline void DAC_Init(LPC_DAC_TypeDef* pDAC){
		//Power: DAC is always connected to VDD
		//Peripheral Clock: PCLK
		//Enable DAC Pins
		DAC_Pin(LPC_DAC);
		LPC_DAC->CR |= (0x00 << 16);						// BIAS: Settling time of DAC is 1us with mx current 700uA
		LPC_DAC->CTRL |= (0x00 << 0)						// DMA Request enabled
					  |(0x01 << 1)							// Double buffering enabled
					  |(0x01 << 2);							// Time-out operation
	}

	inline void DAC_UpdateValue(LPC_DAC_TypeDef *pDAC, uint32_t val){
		uint32_t temp;

		temp = pDAC->CR & DAC_BIAS_EN;
		temp |= DAC_VALUE(val);
		/* Update value */
		pDAC->CR = temp;
	}


private:
	/******************************************************************************
	\brief		DAC Pin initialization
	\param		pDAC	Pointer to DAC Peripheral
	\note		none
	******************************************************************************/
	inline void DAC_Pin(LPC_DAC_TypeDef* pDAC){
		LPC_IOCON->P0_26 |= (0x02 << 0)						// FUNC: Select DAC_Out edge pin 18
						 |(0x00 << 3)						// Mode: must set to inactive
						 |(0x00 << 7);						// ADMODE: Analog Mode selceted
		LPC_GPIO0->DIR |= (0x01 << 26);						// P0_26 pin is output
	}
};

#endif /* DAC_H_ */
