/*
 * int.c
 *
 *  Created on: Feb 15, 2025
 *      Author: ben
 */
#include "int.h"

#define STEPS 512
uint32_t dac_ramp[STEPS];

int counts_max = 0;
void DACLut() {

	int pri_OCD = GetValue(MAX_PRI_I);
	int ct_ratio = GetValue(CT_FACTOR);
	float volts_fb = (float) pri_OCD * (float) ct_ratio; // uV/A * A = uV
	volts_fb = volts_fb / 1000000.0; // V
	volts_fb = volts_fb / 2.0; // 1k extra resistor
	counts_max = (int) (volts_fb / VREF * 4095.0);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, counts_max);


/*
	int64_t a_per_us = GetValue(I_RAMP);
	int64_t I_start = GetValue(I_START);

	if (I_start == 0) {
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, counts_max);
	} else {
		float I_step = (pri_OCD - I_start) / STEPS;
		for (int i = 0; i < STEPS; i++) {
			if (a_per_us == 0 || I_start >= pri_OCD) {
				dac_ramp[i] = counts_max;
			} else {
				float amps = I_step * i + I_start;
				volts_fb = (float) amps * (float) ct_ratio; // uV/A * A = uV
				volts_fb = volts_fb / 1000000.0; // V
				volts_fb = volts_fb / 2.0; // 1k extra resistor
				int counts = (int) (volts_fb / VREF * 4095.0);
				if (counts < 0) counts = 0;
				if (counts > counts_max) counts = counts_max;
				dac_ramp[i] = counts;
			}
		}


		float ramp_time = (pri_OCD - I_start) / a_per_us / (float) STEPS; // in us
		if (ramp_time < 1) ramp_time = 1;
		TIM2->ARR = (uint32_t) (ramp_time * 170.0);

	}
*/
}

uint32_t of_counter = 0;
void TIM2Overflow() {
	of_counter++;
	if (of_counter >= STEPS && GetValue(I_START) != 0) {
		//HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_2);
	}
	if (!(INT_IN_GPIO_Port->IDR & INT_IN_Pin)) {
		HAL_TIM_Base_Stop(&htim2);
		HAL_TIM_Base_Stop(&htim16);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
	}
}

void TIM16Overflow() {
	if (TIM16->ARR != 0) {
		SetFault(FAULT_ONTIME);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == INT_IN_Pin) {
		TIM2->CNT = 0;
		TIM16->CNT = 0;
		if (TIM16->ARR != 0) {
			HAL_TIM_Base_Start_IT(&htim16);
		}
		if (GetValue(I_START) != 0) {
			//HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_2, dac_ramp, STEPS, DAC_ALIGN_12B_R);
		}
		HAL_TIM_Base_Start_IT(&htim2);
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
		of_counter=  0;
	}
}
