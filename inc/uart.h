/**********************************************************************************************************
 * Project     : LPC4088FET208
 * Author      :
 * Version     :
 * Copyright   : $(ISAT)
 * Description : uart definition
 **********************************************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "LPC407x_8x_177x_8x.h"

#define RDR        							(1 << 0)	    //Line status: Receive data ready
#define THRE			       				(1 << 5)		//Transmit holding register
#define UART_RBR_MASKBIT    				(0xFF)		    // UART Received Buffer mask bit (8 bits)
#define LINE_FEED   						0x0A
#define CARRIAGE_RETURN 					0x0D
#define ENTER       						CARRIAGE_RETURN
#define ESC									0x1B

/******************************************************************************
 * Baudrate Calculation: User Manual S.508
 *
 * UARTn_Baudrate = PCLK / (16 x (256 x DLM + DLL) + (1 + (DIVADDL/MULVAL))
 *
 * PCLK = 120MHz
 * DLM = 0
 * DLL = 65		115200Baud
 * DLL = 775	9600 Baud
 * DIVADDL = 0
 * MULVAL = 1
 *
 * --> Baudrate = 115384,615 ~ 115200
*******************************************************************************/

class UART{
public:
	/******************************************************************************
	\brief		Initialize UART-Peripheral
	\param		*aurt		UART-Peripheral
	\note		Clocks up peripheral and initialize pins
	******************************************************************************/
	inline void UART3_Init(){
		LPC_SC->PCONP |= (1 << 25);								// Enable Power & CLock for UART3
		LPC_IOCON->P0_25 = (0x03 << 0)							// UART3 TX	p[0][25]
					  |(0x01 << 7);								// Digital Mode
		LPC_IOCON->P0_26 = (0x03 << 0)  						// UART3 RX	p[0][26]
					  |(0x01 << 7);								// Digital Mode
		LPC_UART3->LCR = (0x03 << 0)							// Word Length: 8 Bit
						 |(0X00 << 2)							// Stop Bit: 1 Bit
						 |(0X00 << 3)							// Parity Enable: disabled
						 |(0X01 << 7);							// Divisor Latch: enabled
		LPC_UART3->DLL = 65;									// Divisor Latch: 65 for 115200 Baud 20:9600
		LPC_UART3->DLM = 0;										// Divisor Latch: 0 for 115200 Baud  3:9600
		LPC_UART3->FCR = (0x01 << 0)							// FIFO enable: enabled
					|(0x01 << 1)								// RX FIFO reset: enabled
					|(0x01 << 2);								// TX FIFO reset: enabled
		LPC_UART3->FDR = (0x00 << 0)							// Baudrate prescaler Value: DIVADDL = 0
					|(0x01 << 4);								// Baudrate prescaler Value: MULVAL = 1
		LPC_UART3->LCR &= ~(0x01 << 7);							// disable to lock Baud - without this no UART communication
	}

	/******************************************************************************
	\brief		Check if UART is busy
	\param		*aurt		UART-Peripheral
	\note
	******************************************************************************/
	inline uint32_t status(LPC_UART_TypeDef *uart){
			return uart->LSR;									//returns if UART is busy or not
	}

	/******************************************************************************
	\brief		Send Bytes to Console
	\param		*aurt		UART-Peripheral
	\note		transmit characters
	******************************************************************************/
	inline void sendByte(LPC_UART_TypeDef *uart, uint8_t data){
			uart->THR = (uint32_t) data;						//The next character to be transmitted is written here
	}

	/******************************************************************************
	\brief		Send Data
	\param		*aurt		UART-Peripheral
	\note		function status & sendByte must be knwon
	******************************************************************************/
	inline int send(LPC_UART_TypeDef *uart, const void *data, int numbytes){
		int sent = 0;
		uint8_t *ptr = (uint8_t *) data;
		while((sent < numbytes) && ((status(uart) & THRE) != 0)){
			sendByte(uart, *ptr);
			ptr++;
			sent++;
		}
		return sent;
	}

	/******************************************************************************
	\brief		Print string to console
	\param		*aurt		UART-Peripheral
	\note		function send must be knwon
	******************************************************************************/
	inline int printf(LPC_UART_TypeDef *uart, const void *data, int numBytes){
		int pass, sent = 0;
		uint8_t *p8 = (uint8_t *) data;

		while (numBytes > 0) {
			pass = send(uart, p8, numBytes);
			numBytes -= pass;
			sent += pass;
			p8 += pass;
		}
		return sent;
	}

	/******************************************************************************
	\brief		Print Char to console
	\param		*aurt		UART-Peripheral
	\note		function printf must be knwon
	******************************************************************************/
	void printfChar (LPC_UART_TypeDef *uart, uint8_t data){

		printf(uart, &data, 1);
	}
	/******************************************************************************
	\brief		Print 32bit numbers to console
	\param		*aurt		UART-Peripheral
	\note		function printfChar must be knwon
	******************************************************************************/
	inline void printHex32(LPC_UART_TypeDef *uart, uint32_t hexnum){
		uint8_t temp, i;
		i = 7;

		do{
			temp = (hexnum >> (4*i)) & 0x0F;

			printfChar(uart, (temp > 9) ? ('A' + temp - 10) : ('0' + temp));
		}while (i--);
	}


	/******************************************************************************
	\brief		Print 16bit hex numbers to console
	\param		*aurt		UART-Peripheral
	\note
	******************************************************************************/
	inline void printHex16(LPC_UART_TypeDef *uart, uint32_t hexnum){
		uint8_t temp, i;
		i = 3;
		do{
			temp = (hexnum >> (4*i)) & 0x0F;
			if (temp > 9){
				temp = 'A' + temp - 10;
			}
			else{
				temp = '0' + temp;
			}
			while((uart->LSR & THRE) == 0);
			uart->THR = (uint32_t) temp;
		}while (i--);
	}

	/******************************************************************************
	\brief		Print 32 bit decimal numbers to console
	\param		*aurt		UART-Peripheral
	\note		function printfChar must be knwon
	******************************************************************************/
	inline void printfDec32(LPC_UART_TypeDef *uart, uint32_t decnum){

		uint8_t c1=decnum%10;
		uint8_t c2=(decnum/10)%10;
		uint8_t c3=(decnum/100)%10;
		uint8_t c4=(decnum/1000)%10;
		uint8_t c5=(decnum/10000)%10;
		uint8_t c6=(decnum/100000)%10;
		uint8_t c7=(decnum/1000000)%10;
		uint8_t c8=(decnum/10000000)%10;
		uint8_t c9=(decnum/100000000)%10;
		uint8_t c10=(decnum/1000000000)%10;
		printfChar(uart, '0'+c10);
		printfChar(uart, '0'+c9);
		printfChar(uart, '0'+c8);
		printfChar(uart, '0'+c7);
		printfChar(uart, '0'+c6);
		printfChar(uart, '0'+c5);
		printfChar(uart, '0'+c4);
		printfChar(uart, '0'+c3);
		printfChar(uart, '0'+c2);
		printfChar(uart, '0'+c1);
	}

	/******************************************************************************
	\brief		Read input from console
	\param		*aurt		UART-Peripheral
	\note		function printfChar must be knwon
	******************************************************************************/
	inline char read(LPC_UART_TypeDef *uart){
		LPC_UART3->FCR = (1 << 1);		//Reset RX FIFO
		LPC_GPIO1->PIN |= (1UL << 18);

		if(uart->LSR & RDR){
			LPC_GPIO1->PIN &= ~(1UL << 18);	//LED1 on
			return uart->RBR;
		}
		return '\0';
	}
};



#endif /* UART_H_ */
