/**********************************************************************************************************
 * Project     : LPC4088FET208
 * Author      :
 * Version     :
 * Copyright   : $(ISAT)
 * Description : GPDMA definition
 **********************************************************************************************************/

#ifndef GPDMA_H_
#define GPDMA_H_

#include "LPC407x_8x_177x_8x.h"
#include <stdint.h>
#include <stdio.h>

#define SRAMBANK0							(unsigned int *) 0x20000000				//Start address SRAM Bank0
#define SRAMBANK1							(unsigned int *)0x20004000				//Start address SRAM Bank1
#define DstEnd0								(unsigned int *)0x20003FFB 				//0x20001FFF
#define DstEnd1								(unsigned int *)0x20007FFB
#define MATRIXARB 							0x400FC188 								//S19 Matrix Arbitration register
#define MASKPORT1							0x3E7FE713          					//MAsk for Port1 Pins
#define DMA_SIZE							0x01									//DMA transfersize
#define c_Diff_RAM							(unsigned int *)0x10007000
#define c_Diff_RAM_END						(unsigned int *)0x1000AFFF
#define M2M									0x00
#define M2P									0x01
#define P2M									0x02
#define P2P									0x03


volatile uint32_t DMATCCount = 0;
volatile uint32_t DMAErrCount = 0;
volatile uint32_t DMA0Done = 0;
volatile uint32_t DMA1Done = 0;
unsigned int volatile *port0 = (unsigned int *) SRAMBANK0;					// *port = value;  //write to port
unsigned int volatile *port1 = (unsigned int *) SRAMBANK1;					// value = *port;  //read from port
unsigned int volatile *v_Diff_RAM = (unsigned int *) c_Diff_RAM;			// value = *port;  //read from port


//ADC zu RAM
const unsigned int c_DMA_Control_Measure = 0x001			// Transfersize: Size of transfer
		  	  	  	  	  	 |(0x00 << 12)					// SBsize: 1
						  	 |(0x00 << 15)					// DBsize: 1
							 |(0x02 << 18)					// SWidth: Word(32-Bit=0x02)
							 |(0x02 << 21)					// DWidth: Word(32-Bit)
							 |(0x01 << 27);					// Destination Increment: enabled
		  	  	  	  	  	 //|(0x01 << 28)				//Protection 1 - access is in privileged mode. kein effekt
		  	  	  	  	  	 //|(0x01 << 29)				//Protection 2 - buffered.						kein effekt
		  	  	  	  	  	 //|(0x01 << 30);				//Protection 3 chached						kein effekt
		  	  	  	  	  	 //|(0x01 << 31);				// Terminal Count Interrupt: enabled

const unsigned int c_DMA_Config_Measure = (0x03 << 1) 		//SRCPERIPHERAL Source Peripheral: T1.MAT1 Quelle
							 |(0x00 << 6)					// Destination Peripheral: none wird ignoriert weil Ziel=Speicher
							 |(0x06 << 11)					// Transfertype: 0x02 Peripheral to memory DMA controller; 0x06 Peripheral to memory Source peripheral;
							 //|(0x01 << 14)				// Interrupt error Mask: enabled
							 //|(0x01 << 15)				// Terminal Count Interrupt Mask: enabled
							 |(0x01 << 0);					// Channel enable: enabled

//Ram zu Ram verschieben
const unsigned int c_DMA_Control_Copy = 0xFFF				// Transfersize: Size of transfer
		  	  	  	  	  	 |(0x00 << 12)					// SBsize: 1
							 	|(0x00 << 15)				// DBsize: 1
								|(0x02 << 18)				// SWidth: Word(32-Bit=0x02)
								|(0x02 << 21)				// DWidth: Word(32-Bit)
								|(0x01 << 26)				// Source Increment: enabled
								|(0x01 << 27)				// Destination Increment: enabled
								|(0x01 << 31);				// Terminal Count Interrupt: enabled
const unsigned int c_DMA_Config_Copy = (0x00 << 1) 			//SRCPERIPHERAL Source Peripheral: T1.MAT1 Quelle
								|(0x00 << 6)				// Destination Peripheral: none wird ignoriert weil Ziel=Speicher
								|(0x00 << 11)				// Transfertype: 0x02 Peripheral to memory DMA controller; 0x06 Peripheral to memory Source peripheral; 0x00 M to M by DMA Controller, 0b101 M to M by Source
								|(0x01 << 15)				// Terminal Count Interrupt Mask: enabled
								|(0x01 << 0);				// Channel enable: enabled

const unsigned int c_DMA_Config_HALT = c_DMA_Config_Measure|(0x01 << 18);		//halt bit zum anhalten beim Interupt


class GPDMA{
public:
	/******************************************************************************
	\brief		DMA Interrupt Handler
	\param		none
	\note		not necessary at automatic transfers
	******************************************************************************/
	inline void DMA_IRQHandler(void){
		LPC_GPDMA->IntTCClear = 0x01;								//Clear TC Interrupt flag
		LPC_GPDMA->IntErrClr = 0x01;
		LPC_GPDMA->IntErrClr = 0x01;
		LPC_GPDMACH0->CConfig = 0x00;
	}


	/******************************************************************************
	\brief		Initialize the DMA Controller
	\param		none
	\note		Clocks up the controller, clears all pending interrupts
	******************************************************************************/
	inline void DMA_Init(void){

		LPC_SC->PCONP |= (1 << 29);									//Power & Clock: enabled
		LPC_SC->DMAREQSEL = (1 << 3);								//DMA Request Peripheral: T1.MAT1
		LPC_GPDMA->IntTCClear = 0x01;
		LPC_GPDMA->IntErrClr = 0x01;
		LPC_GPDMA->Config = 0x01;									//DMA Controller: enabled/ AHB 0= little-endian mode (default) 1 = big-endian mode.
		while(!(LPC_GPDMA->Config & 0x01));							//wait till Controller ready

		NVIC_EnableIRQ(DMA_IRQn);									//activate Interrupt
	}


	/******************************************************************************
	\brief		Transfer Data from src to dst
	\param		*p_DstAddr			Pointer to destination address
	\note		If Controller finished transfer it returns true
	******************************************************************************/
	inline uint32_t DMA_Measure_Ram0( unsigned int volatile * p_DstAddr){

		LPC_GPDMACH0->CSrcAddr = (uint32_t) &(LPC_GPIO1->PIN);		// source address of the data to be transferred
		LPC_GPDMACH0->CDestAddr = (uintptr_t) p_DstAddr;			// destination address of the data to be transferred.
		LPC_GPDMACH0->CLLI = 0x00;									// Linked List Item: disabled
		LPC_GPDMACH0->CControl = c_DMA_Control_Measure;
		LPC_GPDMACH0->CConfig = c_DMA_Config_Measure;
		return(true);
	}


	/******************************************************************************
	\brief		Transfer Data from src to dst
	\param		*p_SrcAddr			Pointer to destination address
	\param		*p_DstAddr			Pointer to source address
	\note		If Controller finished transfer it returns true
	******************************************************************************/
	inline uint32_t DMA_Measure_Copy( unsigned int volatile * p_SrcAddr, unsigned int volatile * p_DstAddr){

		LPC_GPDMACH0->CSrcAddr = (uintptr_t)p_SrcAddr;				// source address of the data to be transferred
		LPC_GPDMACH0->CDestAddr = (uintptr_t) p_DstAddr;			// destination address of the data to be transferred.
		LPC_GPDMACH0->CLLI = 0x00;									// Linked List Item: disabled
		LPC_GPDMACH0->CControl = c_DMA_Control_Copy;
		LPC_GPDMACH0->CConfig = c_DMA_Config_Copy;
		LPC_GPDMA->SoftBReq = 0x01;									//start der Übertragung

		return(true);
	}




// !class GPDMA
};

























#endif /* !GPDMA_H_ */
