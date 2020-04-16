/*
 * spifi.h
 *
 *  Created on: 28.02.2020
 *      Author: tim5672s
 */
#include <stdio.h>
#include <stdint.h>

#ifndef SPIFI_H_
#define SPIFI_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef struct{
	__IO uint32_t CTRL;
	__IO uint32_t CMD;
	__IO uint32_t ADDR;
	__IO uint32_t IDATA;
	__IO uint32_t CLIMIT;
	__IO uint32_t DATA;
	__IO uint32_t MCMD;
	__IO uint32_t STAT;
}LPC_SPIFI_T;




#ifdef __cplusplus
}
#endif
#endif /* SPIFI_H_ */
