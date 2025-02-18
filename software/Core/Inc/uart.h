/*
 * uart.h
 *
 *  Created on: Jul 11, 2024
 *      Author: flyin
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "tterm/TTerm.h"
#include "main.h"
#include "stm32g4xx_hal.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void UartInit();
extern TERMINAL_HANDLE * terminal;
void printer(char * format, ...);

#endif /* INC_UART_H_ */
