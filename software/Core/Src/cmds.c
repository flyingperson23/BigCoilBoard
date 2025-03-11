/*
 * cmds.c
 *
 *  Created on: Jul 11, 2024
 *      Author: flyin
 */

#include "cmds.h"

Var * tempVar;
int tempInt;

uint8_t CMD_telem(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args) {
	ttprintf("Bus Voltage: %fVdc\r\n", (int) vbus);
	ttprintf("Line Voltage: %fVac\r\n", (int) vac_rms);
	ttprintf("Line Current: %fA\r\n", (int) I_L_rms);
	ttprintf("Temps: %i %i %i %i %i %i\r\n", (int) temps[0], (int) temps[1], (int) temps[2], (int) temps[3], (int) temps[4], (int) temps[5]);
	//ttprintf("ADC Readings: %i %i %i\r\n", vbus_buf[0], vac_buf[0], I_L_buf[0]);
	return TERM_CMD_EXIT_SUCCESS;
}

uint8_t CMD_get(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args) {
	if (argCount == 0) {
		for (int i = 0; i < NUM_VARS; i++) {
			ttprintf("%s: %i", GetVar(i)->name, (int) GetVar(i)->value);
			ttprintf("%s \r\n", GetVar(i)->suffix);
		}
		return TERM_CMD_EXIT_SUCCESS;
	}
	if (argCount != 1) {
		ttprintf("Usage: get [name]\r\n");
		return TERM_CMD_EXIT_SUCCESS;
	}
	if (GetIndex(args[0]) == -1) {
		ttprintf("Var not found: %s\r\n", args[0]);
		return TERM_CMD_EXIT_SUCCESS;
	}
	tempVar = GetVar(GetIndex(args[0]));
	ttprintf("%s: %i%s\r\n", args[0], tempVar->value, tempVar->suffix);
	return TERM_CMD_EXIT_SUCCESS;
}

uint8_t CMD_set(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args) {
	if (argCount == 1 && !strcmp(args[0], "default")) {
		ttprintf("Set all to default\r\n");
		for (int i = 0; i < NUM_VARS; i++) {
			GetVar(i)->value = GetVar(i)->default_value;
		}
		WriteVars();
		FillVars();
		return TERM_CMD_EXIT_SUCCESS;
	}
	if (argCount != 2) {
		ttprintf("Usage: set [name] [value]\r\n");
		return TERM_CMD_EXIT_SUCCESS;
	}
	if (GetIndex(args[0]) == -1) {
		ttprintf("Var not found: %s\r\n", args[0]);
		return TERM_CMD_EXIT_SUCCESS;
	}
	tempVar = GetVar(GetIndex(args[0]));
	tempInt = atoi(args[1]);
	if (tempInt < tempVar->min || tempInt > tempVar->max) {
		ttprintf("Valid range: %i-%i\r\n", tempVar->min, tempVar->max);
		return TERM_CMD_EXIT_SUCCESS;
	}
	tempVar->value = tempInt;
	ttprintf("Set %s to %i%s\r\n", tempVar->name, tempInt, tempVar->suffix);
	WriteVars();
	FillVars();
	return TERM_CMD_EXIT_SUCCESS;

}

uint8_t CMD_fault(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args) {
	if (argCount != 1) {
		ttprintf("Usage: fault [get/clear]\r\n");
		return TERM_CMD_EXIT_SUCCESS;
	}

	if (!strcmp(args[0], "get")) {
		if (fault == 0) {
			ttprintf("No faults active");
		} else {
			ttprintf("Faults active:\r\n");
			if (fault & FAULT_OC) {
				ttprintf(" -Overcurrent\r\n");
			}
			if (fault & FAULT_OV) {
				ttprintf(" -Overvoltage\r\n");
			}
			if (fault & FAULT_OT) {
				ttprintf(" -Overtemp\r\n");
			}
			if (fault & FAULT_UV) {
				ttprintf(" -Undervoltage\r\n");
			}
			if (fault & FAULT_ONTIME) {
				ttprintf(" -Ontime\r\n");
			}
			if (fault & FAULT_MANSTOP) {
				ttprintf(" -Manual Stop\r\n");
			}
		}
		return TERM_CMD_EXIT_SUCCESS;
	}
	if (!strcmp(args[0], "clear")) {
		fault = 0;
		return TERM_CMD_EXIT_SUCCESS;
	}

	ttprintf("Usage: fault [get/clear]\r\n");
	return TERM_CMD_EXIT_SUCCESS;
}

uint8_t CMD_vbus(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args) {
	if (argCount == 1) {
		if (!strcmp(args[0], "get")) {
			ttprintf("vbus: %i\r\n", (int) (1000 * vbus));
			ttprintf("vbus target: %i\r\n", (int) (1000 * vbus_target));
			ttprintf("I_L: %i\r\n", (int) (1000 * I_L_rms));
			ttprintf("I_L_target: %i\r\n", (int) (1000 * I_L_target));
			ttprintf("vac: %i\r\n", (int) (1000 * vac));
			ttprintf("vac_rms: %i\r\n", (int) (1000 * vac_rms));
			ttprintf("vinvsq_rms: %i\r\n", (int) (1000000 * VInvSq_rms));
			ttprintf("CompVOut: %i\r\n", (int) (1000 * CompensatorV.y[0]));
			ttprintf("CompIOut: %i\r\n", (int) (1000 * CompensatorI.y[0]));
			ttprintf("dtc: %i / 1000\r\n", (int) (dtc * 1000));
			ttprintf("enabled: %i", enabled);
			return TERM_CMD_EXIT_SUCCESS;
		} else if (!strcmp(args[0], "off")) {
			ttprintf("Boost: off\r\n");
			vbus_target = 0;
			return TERM_CMD_EXIT_SUCCESS;
		} else {
			int setpoint = atoi(args[0]);
			if (setpoint < 10 || setpoint > GetValue(MAX_OUT_V)) {
				ttprintf("Valid range: %i-%i\r\n", 10, GetValue(MAX_OUT_V));
				return TERM_CMD_EXIT_SUCCESS;
			} else {
				ttprintf("Boost: %iV", setpoint);
				vbus_target = setpoint;
				return TERM_CMD_EXIT_SUCCESS;
			}
		}
	}
	return TERM_CMD_EXIT_SUCCESS;
}

uint8_t CMD_bus(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args) {
	if (argCount != 1) {
		ttprintf("Usage: bus [on/off]\r\n");
		return TERM_CMD_EXIT_SUCCESS;
	}
	if (!strcmp(args[0], "on")) {
		if (bus_status != BUS_OFF) {
			ttprintf("Bus already on");
			return TERM_CMD_EXIT_SUCCESS;
		} else {
			bus_status = BUS_CHARGING;
			ttprintf("Bus: Charging\r\n");
		}
	}
	if (!strcmp(args[0], "off")) {
		vbus_target = 0;
		if (bus_status == BUS_OFF) {
			ttprintf("Bus already off");
			return TERM_CMD_EXIT_SUCCESS;
		} else {
			bus_status = BUS_OFF;
			ttprintf("Bus: Off\r\n");
		}
	}

	return TERM_CMD_EXIT_SUCCESS;
}

uint8_t CMD_kill(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args) {
	SetFault(FAULT_MANSTOP);

	return TERM_CMD_EXIT_SUCCESS;
}


void addCommand(TermCommandFunction function, const char * command, const char * description){
	TERM_addCommand(function, command, description, 0, &TERM_defaultList);
}

void CmdsInit(){
	addCommand(CMD_get, "get", "Gets a variable");
	addCommand(CMD_set, "set", "Sets a variable");
	addCommand(CMD_fault, "fault", "Views/clears faults");
	addCommand(CMD_telem, "telem", "Gets telemetry");
	addCommand(CMD_bus, "bus", "Changes bus status");
	addCommand(CMD_vbus, "vbus", "Changes vbus setpoint");
	addCommand(CMD_kill, "kill", "Manual E-Stop");
	addCommand(CMD_kill, "stop", "Manual E-Stop");
}
