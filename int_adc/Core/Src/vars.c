/*
 * vars.c
 *
 *  Created on: Jul 11, 2024
 *      Author: flyin
 */

#include "vars.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32g4xx_hal.h"
#include "stm32_hal_legacy.h"

Var * vars[NUM_VARS];
int64_t vars_buffer[NUM_VARS];

void AddVars() {
	AddVar("max_pri_i", 1500, "A", MAX_PRI_I, 0, 5000);
	AddVar("max_ac_i", 100, "A", MAX_AC_I, 0, 1000);
	AddVar("max_out_v", 800, "V", MAX_OUT_V, 0, 1000);
	AddVar("max_temp", 60, "C", MAX_TEMP, 0, 200);
	AddVar("max_i_l", 150, "A", MAX_I_L, 0, 1000);
	AddVar("ct_conv_factor", 1812, "u", CT_FACTOR, 0, 1000000); // 1:2814 ct + 5R1 burden - 1A out / 2814A in * 5,100,000uV out / 1A out = 1812 uV/A
	AddVar("ac_ct_conv_factor", 20000, "u", AC_CT_FACTOR, 0, 10000000); // 1 A out / 1000 A in * 20,000,000 uV out / 1 A out = 20,000 uV out / A in
	AddVar("vac_r", 500, "k", VAC_R, 0, 1000000);
	AddVar("vbus_r", 1000, "k", VBUS_R, 0, 1000000);
	AddVar("driver_uvlo", 18, "V", DRIVER_UVLO, 0, 30);
	AddVar("pri_ramp", 0, "A", I_RAMP, 0, 1000000);
	AddVar("pri_start", 0, "A", I_START, 0, 1000000);
	AddVar("max_ot", 1000, "u", MAX_OT, 0, 1000000);
	AddVar("boost_kp", 15, "p", BOOST_KP, 0, 1000000);

}

void AddVar(char * name, int64_t default_value, char * suffix, uint8_t index, int64_t min, int64_t max) {
	Var * newVar = malloc(sizeof(Var));
	memset(newVar, 0, sizeof(Var));
	newVar->default_value = default_value;
	newVar->name = name;
	newVar->suffix = suffix;
	newVar->min = min;
	newVar->max = max;
	vars[index] = newVar;
}

Var * GetVar(uint8_t index) {
	return vars[index];
}

int64_t GetValue(uint8_t index) {
	return GetVar(index)->value;
}

int8_t GetIndex(char* name) {
	for (int i = 0; i < NUM_VARS; i++) {
		if (!strcmp(name, vars[i]->name)) {
			return i;
		}
	}
	return -1;
}

void FillVars() {
	read_flash_memory(MEMORY_START, (uint8_t *)vars_buffer, NUM_VARS * 8);
	for (int i = 0; i < NUM_VARS; i++) {
		vars[i]->value = vars_buffer[i];
	}
}

void WriteVars() {
	for (int i = 0; i < NUM_VARS; i++) {
		vars_buffer[i] = vars[i]->value;
	}
	store_flash_memory(MEMORY_START, (uint8_t *)vars_buffer, NUM_VARS * 8);
}

typedef uint64_t flash_datatype;
#define DATA_SIZE sizeof(flash_datatype)
#define FLASH_BANK1_END 0x0803FFFF
#define FLASH_BANK2_END 0x0807FFFF

void store_flash_memory(uint32_t memory_address, uint8_t *data, uint16_t data_length) {
   uint8_t double_word_data[DATA_SIZE];
   FLASH_EraseInitTypeDef flash_erase_struct = {0};
   HAL_FLASH_Unlock();
   flash_erase_struct.TypeErase = FLASH_TYPEERASE_PAGES;
   flash_erase_struct.Page = (memory_address - FLASH_BASE) / FLASH_PAGE_SIZE;
   flash_erase_struct.NbPages = 1 + data_length / FLASH_PAGE_SIZE;
   if(memory_address >  FLASH_BANK1_END && memory_address < FLASH_BANK2_END ) {
	flash_erase_struct.Banks = FLASH_BANK_2;
   }
   else if(memory_address >  FLASH_BASE && memory_address < FLASH_BANK1_END) {
	flash_erase_struct.Banks = FLASH_BANK_1;
   }
   else {
	printf("illegal memory address \n");
	//UsageFault_Handler();
   }
   uint32_t  error_status = 0;
   HAL_FLASHEx_Erase(&flash_erase_struct, &error_status);
   int i = 0;
   while ( i <= data_length) {
	double_word_data[i % DATA_SIZE] = data[i];
	i++;
	if (i % DATA_SIZE == 0) {
	  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, memory_address + i -
		DATA_SIZE, *((uint64_t *)double_word_data));
	}
   }
   if (i % DATA_SIZE != 0) {
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, memory_address + i
		- i % DATA_SIZE, *((flash_datatype *)double_word_data));
   }
   HAL_FLASH_Lock();
}

void read_flash_memory(uint32_t memory_address, uint8_t *data, uint16_t data_length) {
    for(int i = 0; i < data_length; i++) {
    	*(data + i) = (*(uint8_t *)(memory_address + i));
    }
}

