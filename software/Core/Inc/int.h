/*
 * int.h
 *
 *  Created on: Feb 15, 2025
 *      Author: ben
 */

#ifndef INC_INT_H_
#define INC_INT_H_

#include "main.h"
#include "tc.h"

void TIM2Overflow();
void TIM16Overflow();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void DACLut();

#endif /* INC_INT_H_ */
