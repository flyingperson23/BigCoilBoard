/*
 * uart.c
 *
 *  Created on: Jul 11, 2024
 *      Author: flyin
 */

#include "uart.h"

uint8_t rx_buff[1];
TERMINAL_HANDLE * terminal;

void UartInit() {
  HAL_UART_Receive_IT(&huart3, rx_buff, 1);
  terminal = TERM_createNewHandle(printer, 1, &TERM_defaultList, 0, "root");
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Receive_IT(&huart3, rx_buff, 1);
	TERM_processBuffer(rx_buff, 1, terminal);

}

void printer(char * format, ...) {
	va_list arg;
	va_start (arg, format);

	uint8_t * buff = (uint8_t*) malloc(256);
	int length = vsprintf((char *)buff, format, arg);

	if (length > 256) {
		length = 256;
	}


	HAL_UART_Transmit(&huart3, buff, length, 1000);

	free(buff);

	va_end (arg);
}
