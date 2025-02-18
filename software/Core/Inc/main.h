/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tc.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OCD_OUT_Pin GPIO_PIN_4
#define OCD_OUT_GPIO_Port GPIOA
#define CN2_Pin GPIO_PIN_5
#define CN2_GPIO_Port GPIOA
#define CN1_Pin GPIO_PIN_6
#define CN1_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_8
#define CS_GPIO_Port GPIOA
#define PFC_Pin GPIO_PIN_9
#define PFC_GPIO_Port GPIOA
#define BLED1_Pin GPIO_PIN_10
#define BLED1_GPIO_Port GPIOA
#define BLED2_Pin GPIO_PIN_11
#define BLED2_GPIO_Port GPIOA
#define BLED3_Pin GPIO_PIN_12
#define BLED3_GPIO_Port GPIOA
#define STOP_Pin GPIO_PIN_5
#define STOP_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOB
#define INT_IN_Pin GPIO_PIN_8
#define INT_IN_GPIO_Port GPIOB
#define INT_IN_EXTI_IRQn EXTI9_5_IRQn
#define LED3_Pin GPIO_PIN_9
#define LED3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern DAC_HandleTypeDef hdac1;

extern SPI_HandleTypeDef hspi2;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

extern UART_HandleTypeDef huart3;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
