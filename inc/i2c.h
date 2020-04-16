/**********************************************************************************************************
 * Project     : LPC4088FET208
 * Author      :
 * Version     :
 * Copyright   : $(ISAT)
 * Description : I2C definition
 **********************************************************************************************************/

#ifndef I2C_H_
#define I2C_H_

#include "LPC407x_8x_177x_8x.h"

class I2C{
public:
	/******************************************************************************
	\brief		Initialize I2C Peripheral
	\param		none
	\note
	******************************************************************************/
	inline void Init(void){

		LPC_SC->PCONP |= (1 << 19);								// Power & Clock: enabled
		LPC_IOCON->P0_0 |= (0x03 << 0);							//I2C_SDA
		LPC_IOCON->P0_1 |= (0x03 << 0);							//I2C_SCL

		LPC_I2C1->CONCLR = (0x01 << 2) 							// Assert acknowledge clear bit
						 | (0x01 << 3) 							// i2c clear bit
						 | (0x01 << 5) 							// Start flag clear bit
						 | (0x01 << 6);							// i2c interface disable bit
		LPC_I2C1->SCLL   = 0;
		LPC_I2C1->SCLH   = 5;
		LPC_I2C1->CONSET = (0x01 << 6); 						// i2c enable
	}

	/******************************************************************************
	\brief		Start I2C conversation
	\param		none
	\note
	******************************************************************************/
	inline int Start(void){

	    LPC_I2C1->CONCLR = (0x01 << 3);							// i2c interrpt clear bit
	    LPC_I2C1->CONSET = (0x01 << 5);							// start flag clear bit
	    while(((LPC_I2C1->CONSET) & (0x01 << 3)) == 0);			// wait till flag cleared
	    LPC_I2C1->CONCLR = (0x01 << 5);							// clear start flag bit
	    return LPC_I2C1->STAT;									// return status
	}

	/******************************************************************************
	\brief		Stops I2C conversation
	\param		*p_SSPI		SSPI-Perihperal
	\note
	******************************************************************************/
	inline void Stop(void){

	    LPC_I2C1->CONCLR = (0x01 << 3); 						// clear interrupt flag
	    LPC_I2C1->CONSET = (0x01 << 4);							// set stop flag

	    while(((LPC_I2C1->CONSET) & (1 << 4)) == 0);			// wait till stop flag cleared
	}

	/******************************************************************************
	\brief		Transmit data I2C
	\param		data
	\note
	******************************************************************************/
	inline int TransmitByte(unsigned char data){

		LPC_I2C1->DAT = data;									// holds transmit data
	    LPC_I2C1->CONCLR = (0x01 << 3); 						// clear interrupt flag
	    while(((LPC_I2C1->CONSET) & (0x01 << 3)) == 0);			// wait till interrupt flag cleared

	    return LPC_I2C1->STAT;									//return status
	}

	/******************************************************************************
	\brief		Receive Data I2C
	\param		*ptr
	\param		temp
	\note
	******************************************************************************/
	inline int ReceiveByte(unsigned char *ptr, int temp){

	    if(temp){
	    	LPC_I2C1->CONSET = (0x01 << 2);						// assert acknowledge flag
	    }
	    else{
	    	LPC_I2C1->CONCLR = (0x01 << 2);						// assert acknowledge that bit cleared
	    }
		LPC_I2C1->CONCLR = (0x01 << 3); 						// clear interrupt flag
	    while(((LPC_I2C1->CONSET) & (0x01 << 3)) == 0);			// wait till interrupt flag cleared
	    *ptr = LPC_I2C1->DAT;									// holds receive data
	    return LPC_I2C1->STAT;									//return status
	}

};
#endif /* !I2C_H_ */
