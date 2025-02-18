/*
 * int.c
 *
 *  Created on: Feb 15, 2025
 *      Author: ben
 */
#include "int.h"

#define STEPS 512
uint32_t dac_ramp[STEPS];

void DACLut() {
	int pri_OCD = GetValue(MAX_PRI_I);
	int ct_ratio = GetValue(CT_FACTOR);
	float volts_fb = (float) pri_OCD * (float) ct_ratio; // uV/A * A = uV
	volts_fb = volts_fb / 1000000.0; // V
	volts_fb = volts_fb / 2.0; // 1k extra resistor
	int counts_max = (int) (volts_fb / VREF * 4095.0);


	int64_t a_per_us = GetValue(I_RAMP);
	int64_t max_ot = GetValue(MAX_OT);
	float time_step = (float) pri_OCD / (float) a_per_us;
	for (int i = 0; i < STEPS; i++) {
		if (a_per_us == 0) {
			dac_ramp[i] = counts_max;
		} else {
			float amps = time_step * (float) a_per_us * i;
			volts_fb = (float) amps * (float) ct_ratio; // uV/A * A = uV
			volts_fb = volts_fb / 1000000.0; // V
			volts_fb = volts_fb / 2.0; // 1k extra resistor
			int counts = (int) (volts_fb / VREF * 4095.0);
			if (counts < 0) counts = 0;
			if (counts > counts_max) counts = counts_max;
			dac_ramp[i] = counts;
		}
	}
	if (max_ot != 0) dac_ramp[STEPS - 1] = 0;



	// time_step = us/step = how many us the timer lasts for
	TIM2->ARR = (uint32_t) (time_step * 170.0);

}

uint32_t of_counter = 0;
void TIM2Overflow() {
	of_counter++;
	if (of_counter >= STEPS) {
		HAL_TIM_Base_Stop(&htim2);
		HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
		if (GetValue(MAX_OT) != 0) {
			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == INT_IN_Pin) {
		if (INT_IN_GPIO_Port->IDR & INT_IN_Pin) {
			HAL_TIM_Base_Start(&htim2);
			HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, dac_ramp, STEPS, DAC_ALIGN_12B_R);
		} else {
			HAL_TIM_Base_Stop(&htim2);
			HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);
		}
		of_counter=  0;
	}
}
