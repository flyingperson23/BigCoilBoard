/*
 * boost.c
 *
 *  Created on: Jan 21, 2025
 *      Author: ben
 */

#include "boost.h"

#include "leadlag.h"


LeadLagLPFStruct controller_I_A;

float vbus;
float vbus_target;
float I_L;
float I_L_target;
float vac;

uint16_t vbus_buf[2];
uint16_t vac_buf[1];
uint16_t I_L_buf[1];

uint8_t enabled = 0;
float dtc = 0;

float vbus_conv = 0;
float vac_conv = 0;
float I_L_conv = 0;

uint8_t bit = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {

	if (hadc == &hadc2) {
		vbus = vbus_buf[0] * VREF / 4095.0 * vbus_conv;
	}

	if (hadc == &hadc3) {
		I_L = I_L_buf[0] * VREF / 4095.0 * I_L_conv;
		HAL_GPIO_TogglePin(BLED3_GPIO_Port, BLED3_Pin);
		bit = !bit;
		if (bit) BoostFastLoop();
	}

	if (hadc == &hadc4) {
		vac = vac_buf[0] * VREF / 4095.0 * vac_conv;
	}
}

void ClearRMS(FilterRMS * filter) {
	filter->acc = 0;
	filter->cnt = 0;
	filter->out = 0;
}

void AddRMS(FilterRMS * filter, float value) {
	filter->cnt = filter->cnt + 1;
	filter->acc = filter->acc + (value * value);
}

float CalcRMS(FilterRMS * filter) {
	if (filter->cnt == 0) {
		filter->out = 0;
		filter->acc = 0;
	} else {
		filter->out = sqrt(filter->acc / (float) filter->cnt);
		filter->acc = 0;
		filter->cnt = 0;
	}
	return filter->out;
}

FilterRMS vac_rms;
FilterRMS I_L_rms;

void Boost_Init() {
	vbus_conv = 1000.0 * (float) GetValue(VBUS_R) / R_MEAS;
	vac_conv = 1000.0 * (float) GetValue(VAC_R) / R_MEAS;
	I_L_conv = 1000000.0 / (float) GetValue(AC_CT_FACTOR); // A/V


	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc2, (uint32_t *) vbus_buf, 2);

	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc3, (uint32_t *) I_L_buf, 1);

	HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc4, (uint32_t *) vac_buf, 1);


	LeadLagLPF_reset(&controller_I_A);
	controller_I_A.num_leads = 0;
	controller_I_A.num_lpfs = 0;
	controller_I_A.lag_ki = 0.05f;
	controller_I_A.Kp = 3.0f;

	ClearRMS(&vac_rms);
	ClearRMS(&I_L_rms);

	Boost_Clear();

	TIM1->CCR1 = 0;

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_5);



}

void Boost_Clear() {
	vbus = 0;
	vbus_target = 0;
	I_L = 0;
	I_L_target = 0;
	vac = 0;
	enabled = 0;
	dtc = 0;
}


float v_a = 0;
void FastLoop2() {

	I_L_target = ((vbus_target - vbus)*0.1f) + (I_L_target*0.9f);

	if (fabsf(I_L_target) < 1.0f) {I_L_target = 0.0f;}  // this will shut down the bridge (shuts down if I_a_ref = 0)

	constrain(&I_L_target, 0.0f, GetValue(MAX_AC_I));

	controller_I_A.cmd_lim_min = -150.0f;  // don't add in V out here because we're already FF-ing it
	controller_I_A.cmd_lim_max = 150.0f;

	if (controller_I_A.cmd_lim_min + vac < 0.0f) { controller_I_A.cmd_lim_min = 0.0f - vac; }         // can't apply voltages less than zero
	if (controller_I_A.cmd_lim_max + vac > GetValue(MAX_OUT_V)) { controller_I_A.cmd_lim_max = GetValue(MAX_OUT_V) - vac; } // can't apply voltages greater than Vmax
	if (controller_I_A.cmd_lim_min > 0.0f) { controller_I_A.cmd_lim_min = 0.0f; }
	if (controller_I_A.cmd_lim_max < 0.0f) { controller_I_A.cmd_lim_max = 0.0f; }


	controller_I_A.Kp = ((float) GetValue(BOOST_KP)) / 10.0f;
	if ((I_L_target-I_L) < 0.0f) {
		controller_I_A.Kp = controller_I_A.Kp * 2.5f / 1.5f;
	}


	v_a = LeadLagLPF_Update(&controller_I_A, I_L_target-I_L) + vac;


	// voltage to duty cycle
	if (vbus > 50.0f) {   // prevents /0 errors
		dtc = (v_a / vbus);
		if (vbus > vbus_target) dtc = 0;
		TIM1->CCR1 = (int) (dtc * TIM1->ARR);
	} else {
		TIM1->CCR1 = (int) (v_a*0.1f * TIM1->ARR);
	}
}

void BoostFastLoop() {
	if (I_L > GetValue(MAX_I_L)) {
		SetFault(FAULT_OC);
	}

	if (vbus > GetValue(MAX_OUT_V)) {
		SetFault(FAULT_OV);
	}

	if (enabled && vbus_target > 20.0) {

		FastLoop2();

	} else {
		TIM1->CCR1 = 0;
	}

	AddRMS(&vac_rms, vac);
	AddRMS(&I_L_rms, I_L);

	TIM1->CCR1 = 0.3 * TIM1->ARR;

}

void BoostDisable() {
	//TIM1->CCR1 = 0;
	enabled = 0;
}

void BoostEnable() {
	LeadLagLPF_reset(&controller_I_A);
	enabled = 1;
}
