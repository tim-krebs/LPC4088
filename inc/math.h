/**********************************************************************************************************
 * Project     : LPC4088FET208
 * Author      :
 * Version     :
 * Copyright   :
 * Description : Math definition
 **********************************************************************************************************/

#ifndef MATH_H_
#define MATH_H_

#include "arm_math.h"


/*
* @brief  Floating-point sin_cos function.
* @param[in]  theta    input value in degrees
* @param[out] *pSinVal points to the processed sine output.
* @param[out] *pCosVal points to the processed cos output.
* @return none.
*/
  void arm_sin_cos_f32(float32_t theta, float32_t * pSinVal, float32_t * pCcosVal);




/**
* @brief Correlation of floating-point sequences.
* @param[in] *pSrcA points to the first input sequence.
* @param[in] srcALen length of the first input sequence.
* @param[in] *pSrcB points to the second input sequence.
* @param[in] srcBLen length of the second input sequence.
* @param[out] *pDst points to the block of output data  Length 2 * max(srcALen, srcBLen) - 1.
* @return none.
*/
void arm_correlate_f32(float32_t * pSrcA, uint32_t srcALen, float32_t * pSrcB, uint32_t srcBLen, float32_t * pDst);


/**
* @brief floating-point sinc-function.
* @param[in] start - input value of the array.
* @param[in] stop - input value of the array.
* @param[in] range - step range of the array. 

* @return none.
*/
void sinc(double start, double stop, double range);

#endif /* MATH_H_ */
