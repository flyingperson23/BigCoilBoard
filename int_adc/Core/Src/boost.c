/*
 * boost.c
 *
 *  Created on: Jan 21, 2025
 *      Author: maddie <3
 */

#include "boost.h"

float vbus;
float vbus_target;
float vbus_target_fast;
float I_L;
float vac;

uint16_t v_buf[2];
uint16_t I_L_buf[1];

uint8_t enabled = 0;
float dtc = 0;

float vbus_conv = 0;
float vac_conv = 0;
float I_L_conv = 0;

float L = 0.000055;
float L_adj = 0;
float Kp = 0.04;
float Ki = 0.5;
float f_sw = 9000.0f;


void Calc_L_adj() {
	float max_current = GetValue(MAX_I_L);
	float turns = sqrt(L * 1000000.0f / 0.02f);
	float H = 0.4f * 3.14159265359f * turns * max_current / 33.1f;
    float pu = 0.01f * 1.0f/((0.01f) + ((1.83f * 0.0000001f) * pow(H, 1.46f)));
    float L_saturated = L * pu;
    L_adj = 0.5f * (L_saturated + L);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {

	if (hadc == &hadc3) {
		I_L = I_L_buf[0] * VREF / 4095.0 * I_L_conv;
		BoostFastLoop();
		if (vbus_target < vbus_target_fast) vbus_target += 1;
		if (vbus_target > vbus_target_fast) vbus_target = vbus_target_fast;
	}

	if (hadc == &hadc4) {
		vac = v_buf[0] * VREF / 4095.0 * vac_conv;
		vbus = v_buf[1] * VREF / 4095.0 * vbus_conv;
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
	Calc_L_adj();

	vbus_conv = 1000.0 * (float) GetValue(VBUS_R) / R_MEAS;
	vac_conv = 1000.0 * (float) GetValue(VAC_R) / R_MEAS;
	I_L_conv = 1000000.0 / (float) GetValue(AC_CT_FACTOR); // A/V



	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc3, (uint32_t *) I_L_buf, 1);

	HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc4, (uint32_t *) v_buf, 2);

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
	vbus_target_fast = 0;
	I_L = 0;
	vac = 0;
	enabled = 0;
	dtc = 0;
}

float integrator = 0;
void BoostFastLoop() {
	if (1.5f * I_L > GetValue(MAX_I_L)) { // *2 because measure in the middle
		SetFault(FAULT_OC);
	}

	if (vbus > GetValue(MAX_OUT_V)) {
		SetFault(FAULT_OV);
	}

	if (enabled && vbus_target > 20.0 && vac > 1.0) {
		float error = vbus_target - vbus;
		integrator += error * Ki * 1.0f / f_sw;
		if (integrator > 0.3f) integrator = 0.3f;
		if (integrator < -0.3f) integrator = -0.3f;
		dtc = Kp * error + integrator;
		if (dtc < 0) dtc = 0;

		float max_dtc_1 = f_sw * L_adj * (float) GetValue(MAX_I_L) / vac; // limit max current
		float max_dtc_2 = vbus / (vbus + vac) - 0.05f; // ensure constant dcm
		if (dtc > max_dtc_1) dtc = max_dtc_1;
		if (dtc > max_dtc_2) dtc = max_dtc_2;
		if (vbus > vbus_target) dtc = 0;
		TIM1->CCR1 = (int) (dtc * TIM1->ARR);

	} else {
		TIM1->CCR1 = 0;
	}

	AddRMS(&vac_rms, vac);
	AddRMS(&I_L_rms, I_L);
}

void BoostDisable() {
	TIM1->CCR1 = 0;
	enabled = 0;
}

void BoostEnable() {
	integrator = 0;
	enabled = 1;
}
