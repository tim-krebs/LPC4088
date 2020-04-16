/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC407x_8x.h"
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "math.h"
#include "clock.h"
#include "timer.h"
#include "uart.h"
#include "gpdma.h"
#include "sspi.h"
#include "mathISAT.h"
// TODO: insert other definitions and declarations here

int main(void) {

    // TODO: insert code here
	sinc(-5,5, 0.01);
    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter

    //TODO: Test section
    GPDMA gpdma;
    gpdma.DMA_Init();

    UART uart;
    uart.UART3_Init();

    TIMER timer;
    timer.Timer1_Init();

    SSPI sspi;
    sspi.SetClockRate(LPC_SSP1, 1200000, 3);

    while(1) {
        i++ ;
    }
    return 0 ;
}
