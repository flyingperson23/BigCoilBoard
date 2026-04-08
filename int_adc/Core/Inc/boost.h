/*
 * boost.h
 *
 *  Created on: Jan 21, 2025
 *      Author: maddie <3
 */

#ifndef INC_BOOST_H_
#define INC_BOOST_H_

#include "main.h"
#include "vars.h"
#include "math.h"

extern float vbus;
extern float vbus_target;
extern float vbus_target_fast;
extern float I_L;
extern float vac;

extern float I_L_conv;
extern float vbus_conv;
extern float vac_conv;

extern uint16_t v_buf[2];
extern uint16_t I_L_buf[1];

extern float dtc;

extern uint8_t run;

extern uint8_t enabled;

void BoostFastLoop();
void BoostDisable();
void BoostEnable();

void Boost_Init();
void Boost_Clear();
void Calc_L_adj();

typedef struct {
	float out;
	float acc;
	uint64_t cnt;
} FilterRMS;

extern FilterRMS vac_rms;
extern FilterRMS I_L_rms;

void ClearRMS(FilterRMS * filter);
void AddRMS(FilterRMS * filter, float value);
float CalcRMS(FilterRMS * filter);

#endif /* INC_BOOST_H_ */
