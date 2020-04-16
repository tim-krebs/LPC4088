/**********************************************************************************************************
 * Project     : LPC4088FET208
 * Author      :
 * Version     :
 * Copyright   : 
 * Description : SSPI definition
 **********************************************************************************************************/

#include "LPC407x_8x_177x_8x.h"
#include <stdint.h>
#include <stdio.h>


#ifndef SSPI_H_
#define SSPI_H_


#ifdef __cplusplus
extern "C" {
#endif


#define SCK_Freq								40000000
#define SBIT_CPHA								3
#define SBIT_CPOL								4
#define SBIT_MSTR								5
#define SBIT_LSBF								6
#define SBIT_SPIE								7
#define SBIT_SPIF								7
#define SSP_EN      								((uint32_t) (1 << 1))
#define SSP_DIS									((uint32_t) (0 << 1))
#define SSP_DR_BITMASK(n)   							((n) & 0xFFFF)
#define SSP_CR0_SCR(n)      							((uint32_t) ((n & 0xFF) << 8))
#define SSP_CR1_BITMASK     							((uint32_t) (0x0F))



typedef struct{										// SSPn Structure
	__IO uint32_t CR0;								// Control Register 0. Selects the serial clock rate, bus type, and data size.
	__IO uint32_t CR1;								// Control Register 1. Selects master/slave and other modes.
	__IO uint32_t DR;								// Data Register. Writes fill the transmit FIFO, and reads empty the receive FIFO.
	__I  uint32_t SR;								// Status Register
	__IO uint32_t CPSR;								// Clock Prescale Register
	__IO uint32_t IMSC;								// Interrupt Mask Set and Clear Register
	__I  uint32_t RIS;								// Raw Interrupt Status Register
	__I  uint32_t MIS;								// Masked Interrupt Status Register
	__O  uint32_t ICR;								// SSPICR Interrupt Clear Register
	__IO uint32_t DMACR;								// SSPn DMA control register
} SSP_T;


typedef enum BITS {
	BITS_4 = (3u << 0),								// 4 bits/frame
	BITS_5 = (4u << 0),								// 5 bits/frame
	BITS_6 = (5u << 0),							    	// 6 bits/frame
	BITS_7 = (6u << 0),								// 7 bits/frame
	BITS_8 = (7u << 0),								// 8 bits/frame
	BITS_9 = (8u << 0),								// 9 bits/frame
	BITS_10 = (9u << 0),								// 10 bits/frame
	BITS_11 = (10u << 0),								// 11 bits/frame
	BITS_12 = (11u << 0),								// 12 bits/frame
	BITS_13 = (12u << 0),								// 13 bits/frame
	BITS_14 = (13u << 0),								// 14 bits/frame
	BITS_15 = (14u << 0),								// 15 bits/frame
	BITS_16 = (15u << 0),								// 16 bits/frame
} BITS_T;


typedef enum FRAME_FORMAT {
	FRAMEFORMAT_SPI = (0 << 4),							// Frame format: SPI
	FRAME_FORMAT_TI = (1u << 4),							// Frame format: TI SSI
	FRAMEFORMAT_MICROWIRE = (2u << 4),						// Frame format: Microwire
} FRAME_FORMAT_T;


typedef enum CLOCK_FORMAT {
	CLOCK_CPHA0_CPOL0 = (0 << 6),							// CPHA = 0, CPOL = 0
	CLOCK_CPHA0_CPOL1 = (1u << 6),							// CPHA = 0, CPOL = 1
	CLOCK_CPHA1_CPOL0 = (2u << 6),							// CPHA = 1, CPOL = 0
	CLOCK_CPHA1_CPOL1 = (3u << 6),							// CPHA = 1, CPOL = 1
} CLOCK_MODE_T;


typedef struct SSPI_ConfigFormat {
	BITS_T bits;									// Format config: bits/frame
	CLOCK_MODE_T clockMode;								// Format config: clock phase/polarity
	FRAME_FORMAT_T frameFormat;							// Format config: SPI/TI/Microwire
} SSPI_ConfigFormat;


typedef enum MODE {
	MODE_MASTER = (0 << 2),								// Master mode
	MODE_SLAVE = (1u << 2),								// Slave mode
} MODE_T;



class SSPI{
public:
	/******************************************************************************
	\brief		Set Clock rate fpr SSPI
	\param		*p_SSPI			SSPI-Peripheral
	\param		clk_rate		SSPI clock rate
	\param		prescale		scale clock rate
	\note		set the clock rate for SSPI
	******************************************************************************/
	inline void SetClockRate(LPC_SSP_TypeDef *p_SSPI, uint32_t clk_rate, uint32_t prescale){
		uint32_t temp;
		temp = p_SSPI->CR0 & (~(SSP_CR0_SCR(0xFF)));
		p_SSPI->CR0 = temp | (SSP_CR0_SCR(clk_rate));
		p_SSPI->CPSR = prescale;
	}

	/******************************************************************************
	\brief		Initialize SSPI perihperal
	\param		*p_SSPI			SSPI-periheral
	\param		mode			Master/Slave
	\param		bits			4-16 Bits transfer
	\param		frameFormat		SPI, TI, Microwire
	\param		clockMode		Clock Format
	\note
	******************************************************************************/
	inline void SSPI_Init(LPC_SSP_TypeDef *p_SSPI, uint32_t mode, uint32_t bits, uint32_t frameFormat, uint32_t clockMode){

		LPC_SC->PCONP |= (0x01 << 10);			// enable Power and Clock of peripheral
		LPC_IOCON->P0_6 |= (0x02 << 0);				//SSPI_SSEL
		LPC_IOCON->P0_7 |= (0x02 << 0);				//SSPI_SCK
		LPC_IOCON->P0_8 |= (0x02 << 0);				//SSPI_MISO
		LPC_IOCON->P0_9 |= (0x02 << 0);				//SSPI_MOSI

		LPC_IOCON->P0_7 |= (0x01 << 7);				//Set Pin to Digital Pin
		LPC_IOCON->P0_8 |= (0x01 << 7);				//Set Pin to Digital Pin
		LPC_IOCON->P0_9 |= (0x01 << 7);				//Set Pin to Digital Pin
		p_SSPI->CR1 &= ~(1 << 1);
		p_SSPI->CR1 = (p_SSPI->CR1 & ~(1 << 2))
					| mode;							// select master mode
		p_SSPI->CR0 = (p_SSPI->CR0 & ~0xFF)
					| bits
					| frameFormat
					| clockMode;

		SetClockRate(LPC_SSP1, 100000, 2);
	}

	/******************************************************************************
	\brief		Enable SSPI
	\param		*p_SSPI		SSPI-Perihperal
	\note
	******************************************************************************/
	inline void Enable(LPC_SSP_TypeDef *p_SSPI){
		p_SSPI->CR1 |= SSP_EN;
	}

	/******************************************************************************
	\brief		Initialize UART-Peripheral
	\param		*p_SSPI		SSPI-Peripheral
	\note
	******************************************************************************/
	inline void Disable(LPC_SSP_TypeDef *p_SSP){
		p_SSP->CR1 &= (~SSP_EN) & SSP_CR1_BITMASK;
	}

	/******************************************************************************
	\brief		Send data over SSPI
	\param		*p_SSPI		SSPI-Peripheral
	\param		tx_data		data to send
	\note
	******************************************************************************/
	inline void SendFrame(LPC_SSP_TypeDef *p_SSPI, uint16_t tx_data){
		int statusTNF, satusTNFafter;

		statusTNF = (p_SSPI->SR & (0xFF));
		p_SSPI->DR = SSP_DR_BITMASK(tx_data);
		satusTNFafter = (p_SSPI->SR & (0xFF));
	}

	/******************************************************************************
	\brief		Returns interrupt status
	\param		*p_SSPI		SSPI-Peripheral
	\note
	******************************************************************************/
	inline uint32_t GetIntStatus(LPC_SSP_TypeDef *p_SSPI){
		return p_SSPI->MIS;
	}
};


#ifdef __cplusplus
}
#endif

#endif /* !SSPI_H_ */
