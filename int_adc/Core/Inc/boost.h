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

extern float I_L_conv;
extern float vbus_conv;
extern float vac_conv;

extern float dtc;

extern uint8_t run;

extern uint8_t enabled;

void BoostFastLoop();
void BoostDisable();
void BoostEnable();

void Boost_Init();
void Boost_Clear();


#endif /* INC_BOOST_H_ */
