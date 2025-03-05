/*
 * tc.c
 *
 *  Created on: Jan 14, 2025
 *      Author: ben
 */

#include "tc.h"

uint8_t bus_status = BUS_OFF;
uint16_t therm_readings[5];
float temps[6];
uint16_t aux_adc[1];
uint32_t fault = 0;

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

	HAL_GPIO_WritePin(STOP_GPIO_Port, STOP_Pin, GPIO_PIN_RESET);

	UartInit();
	AddVars();
	FillVars();
	CmdsInit();

	// leds
	HAL_GPIO_WritePin(BLED2_GPIO_Port, BLED2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLED3_GPIO_Port, BLED3_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);


	//HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *) aux_adc, 1);

	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);

	Boost_Init();

	DACLut();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (hadc == &hadc1) {
		temps[5] = (TS_CAL2_TEMP - TS_CAL1_TEMP)/(TS_CAL2 - TS_CAL1) * ((float) aux_adc[0] - TS_CAL1) + TS_CAL1_TEMP;
	}
}

void TC_Loop() {
	HAL_GPIO_TogglePin(BLED1_GPIO_Port, BLED1_Pin);
	HAL_Delay(500);
}

int counter3 = 0;
void TC_Loop_Tim() {
	// check voltage
	if (vbus > GetValue(MAX_OUT_V)) {
		SetFault(FAULT_OV);
	}
	if (I_L_rms > GetValue(MAX_AC_I) + 10) {
		SetFault(FAULT_OC);
	}

	// contactors
	CN_Actuate();

	// thermistors
	for (int i = 0; i < 5; i++) {
		float voltage = (float) therm_readings[i] * VREF / 4095.0;
		float resistance = R_MEAS * (3.3 / voltage - 1.0);
		if (resistance > 1) {
			float temp_1 = log(resistance/5000.0) / 3433.0 + 1.0/298.15;
			if (temp_1 > 0) {
				temps[i] = (1.0 / temp_1) - 273.15;
			}
		}
	}
	for (int i = 0; i < 6; i++) {
		if (temps[i] > GetValue(MAX_TEMP)) {
			SetFault(FAULT_OT);
		}
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
	}

}

uint8_t counter = 0;
float last = 0;
float threshold = 0.5;
void CN_Actuate() {
	if (bus_status == BUS_OFF) {
			HAL_GPIO_WritePin(CN1_GPIO_Port, CN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CN2_GPIO_Port, CN2_Pin, GPIO_PIN_RESET);
		} else if (bus_status == BUS_CHARGING) {
			HAL_GPIO_WritePin(CN1_GPIO_Port, CN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CN2_GPIO_Port, CN2_Pin, GPIO_PIN_RESET);
			counter = (counter + 1) % 1000;
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
	ttprintf("Bus Voltage:       %4iV", (int) vbus);

	TERM_setCursorPos(handle, row_pos + 2, col_pos + 1);
	ttprintf("AC Voltage:        %4iV", (int) vac_rms);


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

	TERM_setCursorPos(handle, row_pos + 6, col_pos + 1);
	ttprintf("RMS power:         %4iW", (int) (vac_rms * I_L_rms));

	TERM_setCursorPos(handle, row_pos + 7, col_pos + 1);
	ttprintf("RMS Current:       %4iA", (int) (I_L_rms));

	TERM_sendVT100Code(handle, _VT100_CURSOR_RESTORE_POSITION,0);
	TERM_sendVT100Code(handle, _VT100_CURSOR_ENABLE,0);
}

void SetFault(uint32_t code) {
	fault = fault | code;
}

void ClearFault(uint32_t code) {
	fault = fault & (~code);
}
