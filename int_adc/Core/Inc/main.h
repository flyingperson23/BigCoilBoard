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
#define LED4_Pin GPIO_PIN_13
#define LED4_GPIO_Port GPIOC
#define THERM5_Pin GPIO_PIN_2
#define THERM5_GPIO_Port GPIOA
#define THERM4_Pin GPIO_PIN_3
#define THERM4_GPIO_Port GPIOA
#define THERM3_Pin GPIO_PIN_4
#define THERM3_GPIO_Port GPIOA
#define OCD_OUT_Pin GPIO_PIN_5
#define OCD_OUT_GPIO_Port GPIOA
#define CN2_Pin GPIO_PIN_6
#define CN2_GPIO_Port GPIOA
#define CN1_Pin GPIO_PIN_7
#define CN1_GPIO_Port GPIOA
#define THERM2_Pin GPIO_PIN_0
#define THERM2_GPIO_Port GPIOB
#define THERM1_Pin GPIO_PIN_1
#define THERM1_GPIO_Port GPIOB
#define INT_IN_Pin GPIO_PIN_2
#define INT_IN_GPIO_Port GPIOB
#define INT_IN_EXTI_IRQn EXTI2_IRQn
#define AUX_Pin GPIO_PIN_12
#define AUX_GPIO_Port GPIOB
#define I_L_Pin GPIO_PIN_13
#define I_L_GPIO_Port GPIOB
#define VAC_SENSE_Pin GPIO_PIN_14
#define VAC_SENSE_GPIO_Port GPIOB
#define VBUS_SENSE_Pin GPIO_PIN_15
#define VBUS_SENSE_GPIO_Port GPIOB
#define PFC_Pin GPIO_PIN_8
#define PFC_GPIO_Port GPIOA
#define V24_SENSE_Pin GPIO_PIN_9
#define V24_SENSE_GPIO_Port GPIOA
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
#define LED3_Pin GPIO_PIN_9
#define LED3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern ADC_HandleTypeDef hadc5;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;
extern DMA_HandleTypeDef hdma_adc4;
extern DMA_HandleTypeDef hdma_adc5;

extern COMP_HandleTypeDef hcomp5;

extern DAC_HandleTypeDef hdac1;
extern DAC_HandleTypeDef hdac4;
extern DMA_HandleTypeDef hdma_dac1_ch1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;

extern UART_HandleTypeDef huart3;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
