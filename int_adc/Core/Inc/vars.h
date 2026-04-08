/*
 * vars.h
 *
 *  Created on: May 11, 2024
 *      Author: maddie <3
 */

#ifndef INC_VARS_H_
#define INC_VARS_H_

#include <stdint.h>

typedef struct {
	int64_t value;
	int64_t default_value;
	char * name;
	char * suffix;
	int64_t min;
	int64_t max;
} Var;

void FillVars();
void WriteVars();
void AddVars();
void AddVar(char* name, int64_t default_value, char * suffix, uint8_t index, int64_t min, int64_t max);
Var * GetVar(uint8_t index);
int8_t GetIndex(char* name);
int64_t GetValue(uint8_t index);

#define MEMORY_START 0x0803F000

#define NUM_VARS 10

#define MAX_PRI_I 0
#define MAX_AC_I 1
#define MAX_OUT_V 2
#define MAX_TEMP 3
#define MAX_I_L 4
#define CT_FACTOR 5
#define AC_CT_FACTOR 6
#define VAC_R 7
#define VBUS_R 8
#define DRIVER_UVLO 9
#define I_RAMP 10
#define I_START 11
#define MAX_OT 12
#define BOOST_KP 13


void store_flash_memory(uint32_t memory_address, uint8_t *data, uint16_t data_length);
void read_flash_memory(uint32_t memory_address, uint8_t *data, uint16_t data_length);

#endif /* INC_VARS_H_ */
