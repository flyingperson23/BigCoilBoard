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

#define B0_I (-0.0022467207347555)
#define B1_I (+0.0000664435172459)
#define B2_I (+0.0023131642520014)
#define A1_I (+0.4829060140104478)
#define A2_I (+0.5170939859895523)

#define B0_V (+39.5756135737575010)
#define B1_V (+0.5787365325810271)
#define B2_V (-38.9968770411764750)
#define A1_V (+1.8139107102320191)
#define A2_V (-0.8139107102320192)

#define B0_VFF (+0.0023769653511714)
#define B1_VFF (+0.0047539307023428)
#define B2_VFF (+0.0023769653511714)
#define A1_VFF (+1.8954477946150896)
#define A2_VFF (-0.9049556560197751)

#endif /* INC_BOOST_H_ */
