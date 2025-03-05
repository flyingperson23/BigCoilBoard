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

float vac_rms = 0;
float VInv_rms = 0;
float VInvSq_rms = 0;
float I_L_rms = 0;

uint16_t vbus_buf[1];
uint16_t vac_buf[1];
uint16_t I_L_buf[1];

uint8_t enabled = 0;
float dtc = 0;

float vbus_conv = 0;
float vac_conv = 0;
float I_L_conv = 0;

void Boost_Init() {
	LeadLagLPF_reset(&controller_I_A);
	controller_I_A.num_leads = 0;
	controller_I_A.num_lpfs = 0;
	controller_I_A.lag_ki = 0.05f;

	controller_I_A.Kp = 3.0f;

	Init2p2zFilter(&CompensatorV, A1_V, A2_V, B0_V, B1_V, B2_V, -9999999.0, 9999999.0);
	Init2p2zFilter(&CompensatorI, A1_I, A2_I, B0_I, B1_I, B2_I, 0.0, 0.8);
	Init2p2zFilter(&FilterVFF, A1_VFF, A2_VFF, B0_VFF, B1_VFF, B2_VFF, -9999.0, 9999.0);
	Init2p2zFilter(&FilterIRMS, A1_VFF, A2_VFF, B0_VFF, B1_VFF, B2_VFF, -9999.0, 9999.0);

	Boost_Clear();

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

	TIM1->CCR1 = 50;
	TIM1->CCR2 = 0;
	HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	vbus_conv = 1000.0 * (float) GetValue(VBUS_R) / R_MEAS;
	vac_conv = 1000.0 * (float) GetValue(VAC_R) / R_MEAS;
	I_L_conv = 1000000.0 / (float) GetValue(AC_CT_FACTOR); // A/V

}

void Boost_Clear() {
	vbus = 0;
	vbus_target = 0;
	I_L = 0;
	I_L_target = 0;
	vac = 0;

	vac_rms = 0;
	VInv_rms = 0;
	VInvSq_rms = 0;
	I_L_rms = 0;

	enabled = 0;
	dtc = 0;

	Reset2p2zFilter(&CompensatorV);
	Reset2p2zFilter(&CompensatorI);
	Reset2p2zFilter(&FilterVFF);
	Reset2p2zFilter(&FilterIRMS);
}

Filter2p2z CompensatorV;
Filter2p2z CompensatorI;
Filter2p2z FilterVFF;
Filter2p2z FilterIRMS; // make sure this works

void fconstrain(float * value, float min, float max) {
	if (*value < min) *value = min;
	if (*value > max) *value = max;
}

float Run2p2zFilter(Filter2p2z * filter, float error) {
	filter->x[2] = filter->x[1];
	filter->x[1] = filter->x[0];
	filter->x[0] = error;

	filter->y[2] = filter->y[1];
	filter->y[1] = filter->y[0];
	filter->y[0] = filter->A1 * filter->y[1] + filter->A2 * filter->y[2] + filter->B0 * filter->x[0] + filter->B1 * filter->x[1] + filter->B2 * filter->x[2];

	if (filter->y[0] < filter->min) filter->y[0] = filter->min;
	if (filter->y[0] > filter->max) filter->y[0] = filter->max;
	return filter->y[0];
}

void Init2p2zFilter(Filter2p2z * filter, float A1, float A2, float B0, float B1, float B2, float min, float max) {
	filter->A1 = A1;
	filter->A2 = A2;
	filter->B0 = B0;
	filter->B1 = B1;
	filter->B2 = B2;
	filter->min = min;
	filter->max = max;
}

void Reset2p2zFilter(Filter2p2z * filter) {
	filter->y[0] = 0;
	filter->y[1] = 0;
	filter->y[2] = 0;
	filter->x[0] = 0;
	filter->x[1] = 0;
	filter->x[2] = 0;
}

void ADC_SPI_Get(uint8_t channel, uint16_t * data) {
	uint8_t tr[3];
	uint8_t rec[3];
	tr[0] = 0b110 + ((channel & 0b100) >> 2);
	tr[1] = (channel & 0b11) << 6;
	tr[2] = 0;


	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, tr, rec, 3, 1000);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);


	*data = ((rec[1] & 0b1111) << 8) | rec[2];
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim1) {
		MeasureTrigger();
	}
}

uint16_t adc_data = 0;
uint8_t bcounter = 0;
void MeasureTrigger() {

	bcounter++;
	if (bcounter >= 20) bcounter = 0;

	if (bcounter % 2 == 0) {
		ADC_SPI_Get(CH_I_L, &adc_data);
		I_L = (float) adc_data * VREF / 4095.0 * I_L_conv;
		BoostFastLoop();
	} else if (bcounter == 1) {
		ADC_SPI_Get(CH_VBUS, &adc_data);
		vbus = (float) adc_data * VREF / 4095.0 * vbus_conv;
		BoostSlowLoop();
	}
	else if (bcounter == 3 || bcounter == 9 || bcounter == 15 || bcounter == 19) {
		ADC_SPI_Get(CH_VAC, &adc_data);
		vac = (float) adc_data * VREF / 4095.0 * vac_conv;
	}
	else if (bcounter == 5) {
		ADC_SPI_Get(CH_TEMP1, &adc_data);
		therm_readings[0] = adc_data;
	}
	else if (bcounter == 7) {
		ADC_SPI_Get(CH_TEMP2, &adc_data);
		therm_readings[1] = adc_data;
	}
	else if (bcounter == 11) {
		ADC_SPI_Get(CH_TEMP3, &adc_data);
		therm_readings[2] = adc_data;
	}
	else if (bcounter == 13) {
		ADC_SPI_Get(CH_TEMP4, &adc_data);
		therm_readings[3] = adc_data;
	}
	else if (bcounter == 17) {
		ADC_SPI_Get(CH_TEMP5, &adc_data);
		therm_readings[4] = adc_data;
	}
}

void BoostSlowLoop() {
	if (vbus > GetValue(MAX_OUT_V)) {
		SetFault(FAULT_OV);
	}

	//if (enabled && vbus_target > 20.0) Run2p2zFilter(&CompensatorV, vbus_target - vbus);
	Run2p2zFilter(&FilterVFF, vac);
	Run2p2zFilter(&FilterIRMS, I_L);

	if (FilterVFF.y[0] == 0) {
		VInvSq_rms = 1.0;
		VInv_rms = 1.0;
		vac_rms = 0.0;
	} else {
		vac_rms = 1.1 * FilterVFF.y[0];
		VInv_rms = 1.0 / vac_rms;
		VInvSq_rms = VInv_rms * VInv_rms;
	}

	I_L_rms = 1.1 * FilterIRMS.y[0];

}

float v_boost_ref = 0;
float i_ref = 0;
float v_a = 0;
void FastLoop2() {
	v_boost_ref = (vbus_target * 0.001f) + (v_boost_ref * 0.999f);
	i_ref = ((v_boost_ref - vbus)*0.1f) + (i_ref*0.9f);

	if (fabsf(i_ref) < 1.0f) {i_ref = 0.0f;}  // this will shut down the bridge (shuts down if I_a_ref = 0)

	constrain(&i_ref, 0.0f, GetValue(MAX_AC_I));

	controller_I_A.cmd_lim_min = -150.0f;  // don't add in V out here because we're already FF-ing it
	controller_I_A.cmd_lim_max = 150.0f;

	if (controller_I_A.cmd_lim_min + vac < 0.0f) { controller_I_A.cmd_lim_min = 0.0f - vac; }         // can't apply voltages less than zero
	if (controller_I_A.cmd_lim_max + vac > GetValue(MAX_OUT_V)) { controller_I_A.cmd_lim_max = GetValue(MAX_OUT_V) - vac; } // can't apply voltages greater than Vmax
	if (controller_I_A.cmd_lim_min > 0.0f) { controller_I_A.cmd_lim_min = 0.0f; }
	if (controller_I_A.cmd_lim_max < 0.0f) { controller_I_A.cmd_lim_max = 0.0f; }


	if ((i_ref-I_L) >= 0.0f) {
		controller_I_A.Kp = 1.5;
	} else {
		controller_I_A.Kp = 2.5;
	}


	v_a = LeadLagLPF_Update(&controller_I_A, i_ref-I_L) + vac;


	// voltage to duty cycle
	if (vbus > 50.0f) {   // prevents /0 errors
		dtc = (v_a / vbus);
		if (vbus > vbus_target) dtc = 0;
		TIM1->CCR2 = (int) (dtc * TIM1->ARR);
	} else {
		TIM1->CCR2 = (int) (v_a*0.1f * TIM1->ARR);
	}

	TIM1->CCR1 = TIM1->CCR2 >> 1;

}

void BoostFastLoop() {
	if (I_L > GetValue(MAX_I_L)) {
		SetFault(FAULT_OC);
	}

	if (enabled && vbus_target > 20.0) {
		/*
		I_L_target = vac * CompensatorV.y[0] * VInvSq_rms;
		if (I_L_target > vac * GetValue(MAX_AC_I) * VInv_rms) {
			I_L_target = vac * GetValue(MAX_AC_I) * VInv_rms;
		}

		//I_L_target = vac / 10.0;
		dtc = Run2p2zFilter(&CompensatorI, I_L_target - I_L);
		//fconstrain(&dtc, 0, 0.8);
		float compare =  ((float) TIM1->ARR) * dtc;

		TIM1->CCR1 = (int) compare >> 1;
		TIM1->CCR2 = (int) compare;*/
		FastLoop2();

		if (TIM1->CCR1 < 50) TIM1->CCR1 = 50;


	} else {
		TIM1->CCR1 = 50;
		TIM1->CCR2 = 0;
	}
}

void BoostDisable() {
	TIM1->CCR1 = 50;
	TIM1->CCR2 = 0;
	enabled = 0;
}

void BoostEnable() {
	LeadLagLPF_reset(&controller_I_A);

	Reset2p2zFilter(&CompensatorV);
	Reset2p2zFilter(&CompensatorI);
	enabled = 1;
}
