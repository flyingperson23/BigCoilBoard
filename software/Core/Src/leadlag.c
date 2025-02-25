#include "leadlag.h"

/*
 * lead, lag, kp, LPF
 * lead: 2 params, zero and pole
 * lag: one param, zero
 * lpf: one pole
 * Kp
 *
 *
 * E -> (lpf) -LPF_E-> (lead) -U-> (lag) -> Y
 */


/*
 * How to use:
 * LPF pole should be somewhere between 0.9 and 0.5 or something
 * lead_z is probably 0.995, lead_p is probs 0.95
 * lag_ki is probably 0.01, use parallel integrator formula
 *
 * integrator is of the form z/(z-1)
 *
 * both lightweight, easy to use, and sufficiently complex....
 *
 */


/*
void LeadLagLPF_init(LeadLagLPFStruct *controller,
		float lpf_pole, unsigned char num_lpfs, float lead_z, float lead_p, unsigned char num_leads, float lag_ki, float Kp, float cmd_lim_min, float cmd_lim_max )
{
	//assign the vars
	controller->lpf_pole = lpf_pole;
	controller->num_lpfs = num_lpfs;
	controller->lead_z = lead_z;
	controller->lead_p = lead_p;
	controller->num_leads = num_leads;
	controller->lag_ki = lag_ki;
	controller->Kp = Kp;
	controller->cmd_lim_min = cmd_lim_min;
	controller->cmd_lim_max = cmd_lim_max;

	controller->E[0] = 0.0f; controller->E[1] = 0.0f;
	controller->LPF_E1[0] = 0.0f; controller->LPF_E1[1] = 0.0f;
	controller->LPF_E2[0] = 0.0f; controller->LPF_E2[1] = 0.0f;
	controller->U1[0] = 0.0f; controller->U1[1] = 0.0f;
	controller->U2[0] = 0.0f; controller->U2[1] = 0.0f;
	controller->integrator = 0.0f;
	controller->Y = 0.0f;

}*/

float LeadLagLPF_Update(LeadLagLPFStruct *controller, float error_in) {

	controller->E[0] = controller->Kp*error_in;

	// low-pass filter
	if (controller->num_lpfs == 1) {
		controller->LPF_E2[0] = (controller->E[0])*(1.0f-controller->lpf_pole) + (controller->LPF_E2[1])*(controller->lpf_pole);
	} else {
		controller->LPF_E1[0] = (controller->E[0])*(1-controller->lpf_pole) + (controller->LPF_E1[1])*(controller->lpf_pole);
		controller->LPF_E2[0] = (controller->LPF_E1[0])*(1.0f-controller->lpf_pole) + (controller->LPF_E2[1])*(controller->lpf_pole);
	}
	//controller->LPF_E2[0] = controller->E[0];
    // lead filter
	if (controller->num_leads == 0) {
		controller->U2[0] = 1.0f*controller->LPF_E2[0];
	} else if (controller->num_leads == 1) {
		controller->U2[0] = 1.0f*controller->LPF_E2[0] - controller->lead_z*controller->LPF_E2[1] + controller->lead_p*controller->U2[1];
	} else {
		controller->U1[0] = 1.0f*controller->LPF_E2[0] - controller->lead_z*controller->LPF_E2[1] + controller->lead_p*controller->U1[1];
		controller->U2[0] = 1.0f*controller->U1[0] - controller->lead_z*controller->U1[1] + controller->lead_p*controller->U2[1];
	}
	//controller->U2[0] = controller->E[0];
	// lag, parallel path integrator
	controller->integrator += controller->lag_ki*controller->U2[0];
	constrain(&(controller->integrator), controller->cmd_lim_min, controller->cmd_lim_max);

	controller->Y = controller->U2[0] + controller->integrator;
	float tempY = controller->Y;
	constrain(&(controller->Y), controller->cmd_lim_min, controller->cmd_lim_max);
	if (tempY != controller->Y) {controller->cmd_saturation = 1;} else { controller->cmd_saturation = 0; }

	controller->E[1] = controller->E[0];
	controller->LPF_E1[1] = controller->LPF_E1[0];
	controller->LPF_E2[1] = controller->LPF_E2[0];
	controller->U1[1] = controller->U1[0];
	controller->U2[1] = controller->U2[0];

    return controller->Y;
    //return controller->LPF_E2[0];

}

void LeadLagLPF_reset(LeadLagLPFStruct *controller) {
	controller->E[0] = 0.0f; controller->E[1] = 0.0f;
	controller->LPF_E1[0] = 0.0f; controller->LPF_E1[1] = 0.0f;
	controller->LPF_E2[0] = 0.0f; controller->LPF_E2[1] = 0.0f;
	controller->U1[0] = 0.0f; controller->U1[1] = 0.0f;
	controller->U2[0] = 0.0f; controller->U2[1] = 0.0f;
	controller->integrator = 0.0f;
	controller->Y = 0.0f;
}



