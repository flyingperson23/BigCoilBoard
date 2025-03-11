#ifndef LEADLAG_H
#define LEADLAG_H

#include "math_ops.h"

typedef struct{
	float lpf_pole;
	float lead_z, lead_p;
	float lag_ki;
	float Kp;
	float cmd_lim_min, cmd_lim_max;

	float E[2];
	float LPF_E1[2];
	float LPF_E2[2];
	float U1[2];
	float U2[2];
	float integrator;
	float Y;

	unsigned char num_leads;
	unsigned char num_lpfs;

	unsigned char cmd_saturation;

	} LeadLagLPFStruct;


float LeadLagLPF_Update(LeadLagLPFStruct *controller, float error_in);

void LeadLagLPF_reset(LeadLagLPFStruct *controller);


#endif
