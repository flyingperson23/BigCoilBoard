/*
 * tc.h
 *
 *  Created on: Jan 14, 2025
 *      Author: ben
 */

#ifndef INC_TC_H_
#define INC_TC_H_

#include <stdint.h>
#include <math.h>
#include "main.h"
#include "vars.h"
#include "boost.h"
#include "tterm/TTerm.h"
#include "uart.h"
#include "cmds.h"
#include "int.h"

extern uint16_t therm_readings[5];
extern float temps[6];
extern uint16_t aux_adc[4];
extern uint8_t bus_status;
extern float v24_value;

void TC_Init();
void TC_Loop();
void TC_Loop_Tim();

#define BUS_OFF 0
#define BUS_CHARGING 1
#define BUS_ON 2

#define VREF 3.3
#define R_MEAS 5100.0

#define TS_CAL1_TEMP 30.0
#define TS_CAL2_TEMP 130.0
#define TS_CAL1 1018.0
#define TS_CAL2 1648.0

extern uint32_t fault;
#define FAULT_OV 1 << 0
#define FAULT_OC 1 << 1
#define FAULT_OT 1 << 2
#define FAULT_UV 1 << 3
#define FAULT_ONTIME 1 << 4
#define FAULT_MANSTOP 1 << 5

#define CH_TEMP1 0
#define CH_TEMP2 1
#define CH_TEMP3 2
#define CH_TEMP4 3
#define CH_TEMP5 4
#define CH_I_L 5
#define CH_VAC 6
#define CH_VBUS 7

void CN_Actuate();
void Overlay_Send(TERMINAL_HANDLE * handle);

void SetFault(uint32_t code);
void ClearFault(uint32_t code);

#endif /* INC_TC_H_ */
