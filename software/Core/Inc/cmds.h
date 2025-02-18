/*
 * cmds.h
 *
 *  Created on: May 11, 2024
 *      Author: flyin
 */

#ifndef INC_CMDS_H_
#define INC_CMDS_H_

#include "tterm/TTerm.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vars.h"
#include "boost.h"
#include "tc.h"

void CmdsInit();

uint8_t CMD_telem(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args);
uint8_t CMD_get(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args);
uint8_t CMD_set(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args);
uint8_t CMD_fault(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args);
uint8_t CMD_vbus(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args);
uint8_t CMD_bus(TERMINAL_HANDLE * handle, uint8_t argCount, char ** args);

#endif /* INC_CMDS_H_ */
