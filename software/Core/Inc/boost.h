/*
 * boost.h
 *
 *  Created on: Jan 21, 2025
 *      Author: ben
 */

#ifndef INC_BOOST_H_
#define INC_BOOST_H_

#include "main.h"
#include "vars.h"

extern float vbus;
extern float vbus_target;
extern float I_L;
extern float I_L_target;
extern float vac;

extern float vac_rms;
extern float VInv_rms;
extern float VInvSq_rms;
extern float I_L_rms;

extern float dtc;

extern uint8_t run;

extern uint8_t enabled;

typedef struct {
	float B0;
	float B1;
	float B2;
	float A1;
	float A2;

	float y[3]; // output
	float x[3]; // input

	float min;
	float max;
} Filter2p2z;

float Run2p2zFilter(Filter2p2z * filter, float error);
void Init2p2zFilter(Filter2p2z * filter, float A1, float A2, float B0, float B1, float B2, float min, float max);
void Reset2p2zFilter(Filter2p2z * filter);

extern Filter2p2z CompensatorV;
extern Filter2p2z CompensatorI;
extern Filter2p2z FilterVFF;
extern Filter2p2z FilterIRMS;

void BoostSlowLoop();
void BoostFastLoop();
void BoostDisable();
void BoostEnable();

void ADC_SPI_Get(uint8_t channel, uint16_t * data);
void MeasureTrigger();

void Boost_Init();
void Boost_Clear();

#define B0_I (+0.0029865587575292)
#define B1_I (+0.0000225164885564)
#define B2_I (-0.0029640422689728)
#define A1_I (+0.7779690592966855)
#define A2_I (+0.2220309407033146)

#define B0_V (+20.6774266292214610)
#define B1_V (+0.1517435977272315)
#define B2_V (-20.5256830314942280)
#define A1_V (+1.9024154974217211)
#define A2_V (-0.9024154974217212)

#define B0_VFF (+0.0006098184237487)
#define B1_VFF (+0.0012196368474975)
#define B2_VFF (+0.0006098184237487)
#define A1_VFF (+1.9487928366059064)
#define A2_VFF (-0.9512321103009015)

#endif /* INC_BOOST_H_ */
