/*
 * tc.c
 *
 *  Created on: Jan 14, 2025
 *      Author: maddie <3
 */

#include "tc.h"

uint8_t bus_status = BUS_OFF;
uint16_t therm_readings[4];
float temps[6];
uint16_t aux_adc[3];
uint16_t adc2[1];
uint32_t fault = 0;
float v24_value = 0;

uint16_t TS_CAL1 = 1;
uint16_t TS_CAL2 = 2;
uint16_t VREFINT = 1;
float VREF = 3.3f;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim6) {
		TC_Loop_Tim();
	}
	if (htim == &htim7) {
		Overlay_Send(terminal);
	}
	if (htim == &htim2) {
		TIM2Overflow();
	}
	if (htim == &htim16) {
		TIM16Overflow();
	}
}

void TC_Init() {
	aux_adc[0] = 0;
	aux_adc[1] = 0;
	aux_adc[2] = 0;

	TS_CAL1 = (*(uint16_t *)(0x1FFF75A8));
	TS_CAL2 = (*(uint16_t *)(0x1FFF75CA));
	VREFINT = (*(uint16_t *)(0x1FFF75AA));

	HAL_GPIO_WritePin(STOP_GPIO_Port, STOP_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);


	UartInit();
	AddVars();
	FillVars();
	CmdsInit();

	// leds
	HAL_GPIO_WritePin(BLED2_GPIO_Port, BLED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLED3_GPIO_Port, BLED3_Pin, GPIO_PIN_RESET);



	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
	HAL_DAC_Start(&hdac4, DAC_CHANNEL_1);


	HAL_ADCEx_Calibration_Start(&hadc5, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc5, (uint32_t *) aux_adc, 3);

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *) therm_readings, 4);

	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc2, (uint32_t *) adc2, 1);

	//HAL_COMP_Start(&hcomp5);

	Boost_Init();

	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);


	DACLut();
}

void TC_Loop() {
	HAL_GPIO_TogglePin(BLED1_GPIO_Port, BLED1_Pin);
	HAL_Delay(500);
}

int counter3 = 0;
void TC_Loop_Tim() {
	if (aux_adc[2] != 0) {
		float vref_new = 3.0f * (float) VREFINT / (float) aux_adc[2];
		if (vref_new < 5.0f && vref_new > 1.5f) {
			VREF = (VREF + vref_new) / 2.0f;
		}
	}

	// contactors
	CN_Actuate();

	// thermistors


	for (int i = 0; i < 4; i++) {
		float voltage = (float) therm_readings[i] * VREF / 4095.0;
		if (voltage > 0) {
			float resistance = R_MEAS * (3.3 / voltage - 1.0);
			if (resistance > 1) {
				float temp_1 = log(resistance/5000.0) / 3433.0 + 1.0/298.15;
				if (temp_1 > 0) {
					temps[i] = (1.0 / temp_1) - 273.15;
				}
			}
		}
	}

	float voltage = (float) adc2[0] * VREF / 4095.0;
	if (voltage > 0) {
		float resistance = R_MEAS * (3.3 / voltage - 1.0);
		if (resistance > 1) {
			float temp_1 = log(resistance/5000.0) / 3433.0 + 1.0/298.15;
			if (temp_1 > 0) {
				temps[4] = (1.0 / temp_1) - 273.15;
			}
		}
	}
	temps[5] = (TS_CAL2_TEMP - TS_CAL1_TEMP)/((float) TS_CAL2 - (float) TS_CAL1) * ((float) aux_adc[1] - (float) TS_CAL1) + TS_CAL1_TEMP;

	for (int i = 0; i < 6; i++) {
		if (temps[i] > GetValue(MAX_TEMP)) {
			SetFault(FAULT_OT);
		}
	}

	v24_value = aux_adc[0] * VREF / 4095.0 * 11.0;
	if (v24_value < GetValue(DRIVER_UVLO) && v24_value > 5.0f) {
		SetFault(FAULT_UV);
	}

	if (fault == 0 && bus_status != BUS_CHARGING) {
		HAL_GPIO_WritePin(STOP_GPIO_Port, STOP_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(STOP_GPIO_Port, STOP_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
	}

	if (fault == 0 && bus_status == BUS_ON) {
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		BoostEnable();
	} else {
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		BoostDisable();
	}

	if (fault != 0) {
		bus_status = BUS_OFF;
	}

	TIM16->ARR = GetValue(MAX_OT);
	counter3++;
	if (counter3 % 100 == 0) {
		DACLut();
		Calc_L_adj();
	}

	if (counter3 % 10 == 0) {
		CalcRMS(&vac_rms);
		CalcRMS(&I_L_rms);
	}

	if (I_L_conv != 0) {
		float counts = (float) GetValue(MAX_I_L) / I_L_conv * 4095.0 / VREF;
		if (counts > 4095) counts = 4095;
		if (counts < 0) counts = 0;
		HAL_DAC_SetValue(&hdac4, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (int) counts);
	}

}

uint8_t counter = 0;
float last = 0;
float threshold = 2.5;
void CN_Actuate() {
	if (bus_status == BUS_OFF) {
			HAL_GPIO_WritePin(CN1_GPIO_Port, CN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CN2_GPIO_Port, CN2_Pin, GPIO_PIN_RESET);
		} else if (bus_status == BUS_CHARGING) {
			HAL_GPIO_WritePin(CN1_GPIO_Port, CN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CN2_GPIO_Port, CN2_Pin, GPIO_PIN_RESET);
			counter = (counter + 1) % 100;
			if (counter == 0) {
				float difference = vbus - last;
				if (difference <= threshold && vbus > 20) {
					bus_status = BUS_ON;
				}
				last = vbus;
			}
		} else if (bus_status == BUS_ON) {
			HAL_GPIO_WritePin(CN1_GPIO_Port, CN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CN2_GPIO_Port, CN2_Pin, GPIO_PIN_SET);
		}
}

void TERM_Box(TERMINAL_HANDLE * handle, uint8_t row1, uint8_t col1, uint8_t row2, uint8_t col2) {
	TERM_setCursorPos(handle, row1, col1);
	TERM_sendVT100Code(handle, _VT100_BACKGROUND_COLOR, _VT100_BLUE);
	ttprintf("\xE2\x95\x94"); //edge upper left
	int i = 0;
	for (i = 1; i < (col2 - col1); i++) {
		ttprintf("\xE2\x95\x90"); //=
	}
	ttprintf("\xE2\x95\x97"); //edge upper right
	for (i = 1; i < (row2 - row1); i++) {
		TERM_setCursorPos(handle, row1 + i, col1);
		ttprintf("\xE2\x95\x91"); //left ||
		TERM_setCursorPos(handle, row1 + i, col2);
		ttprintf("\xE2\x95\x91"); //right ||
	}
	TERM_setCursorPos(handle, row2, col1);
	ttprintf("\xE2\x95\x9A"); //edge lower left
	for (i = 1; i < (col2 - col1); i++) {
		ttprintf("\xE2\x95\x90"); //=
	}
	ttprintf("\xE2\x95\x9D"); //edge lower right
	TERM_sendVT100Code(handle, _VT100_FOREGROUND_COLOR, _VT100_WHITE);
}

void Overlay_Send(TERMINAL_HANDLE * handle) {

	TERM_sendVT100Code(handle, _VT100_CURSOR_SAVE_POSITION,0);
	TERM_sendVT100Code(handle, _VT100_CURSOR_DISABLE,0);

	uint8_t row_pos = 1;
	uint8_t col_pos = 110;
	TERM_Box(handle, row_pos, col_pos, row_pos + 8, col_pos + 25);
	TERM_setCursorPos(handle, row_pos + 1, col_pos + 1);
	ttprintf("Bus Voltage:       %4iV", (int) (vbus));

	TERM_setCursorPos(handle, row_pos + 2, col_pos + 1);
	ttprintf("AC Voltage:        %4iV", (int) (vac_rms.out));


	float hi_temp = -1;
	float avg_temp = 0;
	uint8_t nonzero = 0;
	for (int i = 0; i < 5; i++) {
		if (temps[i] > 0) {
			nonzero++;
			avg_temp += temps[i];
			if (temps[i] > hi_temp) hi_temp = temps[i];
		}
	}
	if (nonzero > 0) avg_temp /=  (float) nonzero;
	else avg_temp = -1;
	TERM_setCursorPos(handle, row_pos + 3, col_pos + 1);
	ttprintf("High Temp:       %4i *C", (int) hi_temp);

	TERM_setCursorPos(handle, row_pos + 4, col_pos + 1);
	ttprintf("Avg Temp:        %4i *C", (int) avg_temp);

	TERM_setCursorPos(handle, row_pos + 5, col_pos + 1);
	ttprintf("Bus status: ");

	if (fault != 0) {
		ttprintf("       Fault");
	} else {
		switch (bus_status) {
			case BUS_OFF:
				ttprintf("         Off");
				break;
			case BUS_CHARGING:
				ttprintf("    Charging");
				break;
			case BUS_ON:
				ttprintf("          On");
				break;
		}
	}


	TERM_setCursorPos(handle, row_pos + 6, col_pos + 1);
	ttprintf("RMS power:         %4iW", (int) (vac_rms.out * I_L_rms.out));

	TERM_setCursorPos(handle, row_pos + 7, col_pos + 1);
	ttprintf("RMS Current:       %4iA", (int) (I_L_rms.out));

	TERM_sendVT100Code(handle, _VT100_CURSOR_RESTORE_POSITION,0);
	TERM_sendVT100Code(handle, _VT100_CURSOR_ENABLE,0);
}

void SetFault(uint32_t code) {
	fault = fault | code;
}

void ClearFault(uint32_t code) {
	fault = fault & (~code);
}
