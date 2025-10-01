/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32g4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                        /**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  HAL_PWREx_DisableUCPDDeadBattery();

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

static uint32_t HAL_RCC_ADC12_CLK_ENABLED=0;
static uint32_t HAL_RCC_ADC345_CLK_ENABLED=0;

/**
  * @brief ADC MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hadc->Instance==ADC1)
  {
    /* USER CODE BEGIN ADC1_MspInit 0 */

    /* USER CODE END ADC1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if(HAL_RCC_ADC12_CLK_ENABLED==1){
      __HAL_RCC_ADC12_CLK_ENABLE();
    }

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN6
    PC1     ------> ADC1_IN7
    PC2     ------> ADC1_IN8
    PC3     ------> ADC1_IN9
    PA0     ------> ADC1_IN1
    */
    GPIO_InitStruct.Pin = THERM1_Pin|THERM2_Pin|THERM3_Pin|THERM4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = THERM5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(THERM5_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC1_MspInit 1 */

    /* USER CODE END ADC1_MspInit 1 */
  }
  else if(hadc->Instance==ADC2)
  {
    /* USER CODE BEGIN ADC2_MspInit 0 */

    /* USER CODE END ADC2_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if(HAL_RCC_ADC12_CLK_ENABLED==1){
      __HAL_RCC_ADC12_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC2 GPIO Configuration
    PB11     ------> ADC2_IN14
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC2_MspInit 1 */

    /* USER CODE END ADC2_MspInit 1 */
  }
  else if(hadc->Instance==ADC3)
  {
    /* USER CODE BEGIN ADC3_MspInit 0 */

    /* USER CODE END ADC3_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC345;
    PeriphClkInit.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    HAL_RCC_ADC345_CLK_ENABLED++;
    if(HAL_RCC_ADC345_CLK_ENABLED==1){
      __HAL_RCC_ADC345_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC3 GPIO Configuration
    PB13     ------> ADC3_IN5
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC3_MspInit 1 */

    /* USER CODE END ADC3_MspInit 1 */
  }
  else if(hadc->Instance==ADC4)
  {
    /* USER CODE BEGIN ADC4_MspInit 0 */

    /* USER CODE END ADC4_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC345;
    PeriphClkInit.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    HAL_RCC_ADC345_CLK_ENABLED++;
    if(HAL_RCC_ADC345_CLK_ENABLED==1){
      __HAL_RCC_ADC345_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC4 GPIO Configuration
    PB12     ------> ADC4_IN3
    */
    GPIO_InitStruct.Pin = VAC_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(VAC_IN_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC4_MspInit 1 */

    /* USER CODE END ADC4_MspInit 1 */
  }
  else if(hadc->Instance==ADC5)
  {
    /* USER CODE BEGIN ADC5_MspInit 0 */

    /* USER CODE END ADC5_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC345;
    PeriphClkInit.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    HAL_RCC_ADC345_CLK_ENABLED++;
    if(HAL_RCC_ADC345_CLK_ENABLED==1){
      __HAL_RCC_ADC345_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC5 GPIO Configuration
    PA9     ------> ADC5_IN2
    */
    GPIO_InitStruct.Pin = V24_SENSE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(V24_SENSE_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN ADC5_MspInit 1 */

    /* USER CODE END ADC5_MspInit 1 */
  }

}

/**
  * @brief ADC MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {
    /* USER CODE BEGIN ADC1_MspDeInit 0 */

    /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC12_CLK_ENABLED--;
    if(HAL_RCC_ADC12_CLK_ENABLED==0){
      __HAL_RCC_ADC12_CLK_DISABLE();
    }

    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN6
    PC1     ------> ADC1_IN7
    PC2     ------> ADC1_IN8
    PC3     ------> ADC1_IN9
    PA0     ------> ADC1_IN1
    */
    HAL_GPIO_DeInit(GPIOC, THERM1_Pin|THERM2_Pin|THERM3_Pin|THERM4_Pin);

    HAL_GPIO_DeInit(THERM5_GPIO_Port, THERM5_Pin);

    /* USER CODE BEGIN ADC1_MspDeInit 1 */

    /* USER CODE END ADC1_MspDeInit 1 */
  }
  else if(hadc->Instance==ADC2)
  {
    /* USER CODE BEGIN ADC2_MspDeInit 0 */

    /* USER CODE END ADC2_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC12_CLK_ENABLED--;
    if(HAL_RCC_ADC12_CLK_ENABLED==0){
      __HAL_RCC_ADC12_CLK_DISABLE();
    }

    /**ADC2 GPIO Configuration
    PB11     ------> ADC2_IN14
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

    /* USER CODE BEGIN ADC2_MspDeInit 1 */

    /* USER CODE END ADC2_MspDeInit 1 */
  }
  else if(hadc->Instance==ADC3)
  {
    /* USER CODE BEGIN ADC3_MspDeInit 0 */

    /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC345_CLK_ENABLED--;
    if(HAL_RCC_ADC345_CLK_ENABLED==0){
      __HAL_RCC_ADC345_CLK_DISABLE();
    }

    /**ADC3 GPIO Configuration
    PB13     ------> ADC3_IN5
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);

    /* USER CODE BEGIN ADC3_MspDeInit 1 */

    /* USER CODE END ADC3_MspDeInit 1 */
  }
  else if(hadc->Instance==ADC4)
  {
    /* USER CODE BEGIN ADC4_MspDeInit 0 */

    /* USER CODE END ADC4_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC345_CLK_ENABLED--;
    if(HAL_RCC_ADC345_CLK_ENABLED==0){
      __HAL_RCC_ADC345_CLK_DISABLE();
    }

    /**ADC4 GPIO Configuration
    PB12     ------> ADC4_IN3
    */
    HAL_GPIO_DeInit(VAC_IN_GPIO_Port, VAC_IN_Pin);

    /* USER CODE BEGIN ADC4_MspDeInit 1 */

    /* USER CODE END ADC4_MspDeInit 1 */
  }
  else if(hadc->Instance==ADC5)
  {
    /* USER CODE BEGIN ADC5_MspDeInit 0 */

    /* USER CODE END ADC5_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_ADC345_CLK_ENABLED--;
    if(HAL_RCC_ADC345_CLK_ENABLED==0){
      __HAL_RCC_ADC345_CLK_DISABLE();
    }

    /**ADC5 GPIO Configuration
    PA9     ------> ADC5_IN2
    */
    HAL_GPIO_DeInit(V24_SENSE_GPIO_Port, V24_SENSE_Pin);

    /* USER CODE BEGIN ADC5_MspDeInit 1 */

    /* USER CODE END ADC5_MspDeInit 1 */
  }

}

/**
  * @brief COMP MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hcomp->Instance==COMP1)
  {
    /* USER CODE BEGIN COMP1_MspInit 0 */

    /* USER CODE END COMP1_MspInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**COMP1 GPIO Configuration
    PA1     ------> COMP1_INP
    */
    GPIO_InitStruct.Pin = OCD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(OCD_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN COMP1_MspInit 1 */

    /* USER CODE END COMP1_MspInit 1 */
  }
  else if(hcomp->Instance==COMP2)
  {
    /* USER CODE BEGIN COMP2_MspInit 0 */

    /* USER CODE END COMP2_MspInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**COMP2 GPIO Configuration
    PA3     ------> COMP2_INP
    PA5     ------> COMP2_INM
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN COMP2_MspInit 1 */

    /* USER CODE END COMP2_MspInit 1 */
  }
  else if(hcomp->Instance==COMP5)
  {
    /* USER CODE BEGIN COMP5_MspInit 0 */

    /* USER CODE END COMP5_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**COMP5 GPIO Configuration
    PB13     ------> COMP5_INP
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN COMP5_MspInit 1 */

    /* USER CODE END COMP5_MspInit 1 */
  }
  else if(hcomp->Instance==COMP6)
  {
    /* USER CODE BEGIN COMP6_MspInit 0 */

    /* USER CODE END COMP6_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**COMP6 GPIO Configuration
    PB11     ------> COMP6_INP
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN COMP6_MspInit 1 */

    /* USER CODE END COMP6_MspInit 1 */
  }

}

/**
  * @brief COMP MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspDeInit(COMP_HandleTypeDef* hcomp)
{
  if(hcomp->Instance==COMP1)
  {
    /* USER CODE BEGIN COMP1_MspDeInit 0 */

    /* USER CODE END COMP1_MspDeInit 0 */

    /**COMP1 GPIO Configuration
    PA1     ------> COMP1_INP
    */
    HAL_GPIO_DeInit(OCD_GPIO_Port, OCD_Pin);

    /* USER CODE BEGIN COMP1_MspDeInit 1 */

    /* USER CODE END COMP1_MspDeInit 1 */
  }
  else if(hcomp->Instance==COMP2)
  {
    /* USER CODE BEGIN COMP2_MspDeInit 0 */

    /* USER CODE END COMP2_MspDeInit 0 */

    /**COMP2 GPIO Configuration
    PA3     ------> COMP2_INP
    PA5     ------> COMP2_INM
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3|GPIO_PIN_5);

    /* USER CODE BEGIN COMP2_MspDeInit 1 */

    /* USER CODE END COMP2_MspDeInit 1 */
  }
  else if(hcomp->Instance==COMP5)
  {
    /* USER CODE BEGIN COMP5_MspDeInit 0 */

    /* USER CODE END COMP5_MspDeInit 0 */

    /**COMP5 GPIO Configuration
    PB13     ------> COMP5_INP
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);

    /* USER CODE BEGIN COMP5_MspDeInit 1 */

    /* USER CODE END COMP5_MspDeInit 1 */
  }
  else if(hcomp->Instance==COMP6)
  {
    /* USER CODE BEGIN COMP6_MspDeInit 0 */

    /* USER CODE END COMP6_MspDeInit 0 */

    /**COMP6 GPIO Configuration
    PB11     ------> COMP6_INP
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

    /* USER CODE BEGIN COMP6_MspDeInit 1 */

    /* USER CODE END COMP6_MspDeInit 1 */
  }

}

/**
  * @brief DAC MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  if(hdac->Instance==DAC1)
  {
    /* USER CODE BEGIN DAC1_MspInit 0 */

    /* USER CODE END DAC1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_DAC1_CLK_ENABLE();
    /* USER CODE BEGIN DAC1_MspInit 1 */

    /* USER CODE END DAC1_MspInit 1 */
  }
  else if(hdac->Instance==DAC2)
  {
    /* USER CODE BEGIN DAC2_MspInit 0 */

    /* USER CODE END DAC2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_DAC2_CLK_ENABLE();
    /* USER CODE BEGIN DAC2_MspInit 1 */

    /* USER CODE END DAC2_MspInit 1 */
  }
  else if(hdac->Instance==DAC4)
  {
    /* USER CODE BEGIN DAC4_MspInit 0 */

    /* USER CODE END DAC4_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_DAC4_CLK_ENABLE();
    /* USER CODE BEGIN DAC4_MspInit 1 */

    /* USER CODE END DAC4_MspInit 1 */
  }

}

/**
  * @brief DAC MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
  if(hdac->Instance==DAC1)
  {
    /* USER CODE BEGIN DAC1_MspDeInit 0 */

    /* USER CODE END DAC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC1_CLK_DISABLE();
    /* USER CODE BEGIN DAC1_MspDeInit 1 */

    /* USER CODE END DAC1_MspDeInit 1 */
  }
  else if(hdac->Instance==DAC2)
  {
    /* USER CODE BEGIN DAC2_MspDeInit 0 */

    /* USER CODE END DAC2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC2_CLK_DISABLE();
    /* USER CODE BEGIN DAC2_MspDeInit 1 */

    /* USER CODE END DAC2_MspDeInit 1 */
  }
  else if(hdac->Instance==DAC4)
  {
    /* USER CODE BEGIN DAC4_MspDeInit 0 */

    /* USER CODE END DAC4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DAC4_CLK_DISABLE();
    /* USER CODE BEGIN DAC4_MspDeInit 1 */

    /* USER CODE END DAC4_MspDeInit 1 */
  }

}

/**
  * @brief TIM_Base MSP Initialization
  * This function configures the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM3)
  {
    /* USER CODE BEGIN TIM3_MspInit 0 */

    /* USER CODE END TIM3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
    /* USER CODE BEGIN TIM3_MspInit 1 */

    /* USER CODE END TIM3_MspInit 1 */
  }
  else if(htim_base->Instance==TIM15)
  {
    /* USER CODE BEGIN TIM15_MspInit 0 */

    /* USER CODE END TIM15_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM15_CLK_ENABLE();
    /* USER CODE BEGIN TIM15_MspInit 1 */

    /* USER CODE END TIM15_MspInit 1 */
  }

}

/**
  * @brief TIM_PWM MSP Initialization
  * This function configures the hardware resources used in this example
  * @param htim_pwm: TIM_PWM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM8)
  {
    /* USER CODE BEGIN TIM8_MspInit 0 */

    /* USER CODE END TIM8_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM8_CLK_ENABLE();
    /* USER CODE BEGIN TIM8_MspInit 1 */

    /* USER CODE END TIM8_MspInit 1 */

  }

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM3)
  {
    /* USER CODE BEGIN TIM3_MspPostInit 0 */

    /* USER CODE END TIM3_MspPostInit 0 */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PC8     ------> TIM3_CH3
    */
    GPIO_InitStruct.Pin = GD_OUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GD_OUT_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM3_MspPostInit 1 */

    /* USER CODE END TIM3_MspPostInit 1 */
  }
  else if(htim->Instance==TIM8)
  {
    /* USER CODE BEGIN TIM8_MspPostInit 0 */

    /* USER CODE END TIM8_MspPostInit 0 */

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**TIM8 GPIO Configuration
    PC6     ------> TIM8_CH1
    */
    GPIO_InitStruct.Pin = PFC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM8;
    HAL_GPIO_Init(PFC_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM8_MspPostInit 1 */

    /* USER CODE END TIM8_MspPostInit 1 */
  }

}
/**
  * @brief TIM_Base MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM3)
  {
    /* USER CODE BEGIN TIM3_MspDeInit 0 */

    /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
    /* USER CODE BEGIN TIM3_MspDeInit 1 */

    /* USER CODE END TIM3_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM15)
  {
    /* USER CODE BEGIN TIM15_MspDeInit 0 */

    /* USER CODE END TIM15_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM15_CLK_DISABLE();
    /* USER CODE BEGIN TIM15_MspDeInit 1 */

    /* USER CODE END TIM15_MspDeInit 1 */
  }

}

/**
  * @brief TIM_PWM MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param htim_pwm: TIM_PWM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM8)
  {
    /* USER CODE BEGIN TIM8_MspDeInit 0 */

    /* USER CODE END TIM8_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM8_CLK_DISABLE();
    /* USER CODE BEGIN TIM8_MspDeInit 1 */

    /* USER CODE END TIM8_MspDeInit 1 */
  }

}

/**
  * @brief UART MSP Initialization
  * This function configures the hardware resources used in this example
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(huart->Instance==USART1)
  {
    /* USER CODE BEGIN USART1_MspInit 0 */

    /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = TX_Pin|RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */

  }

}

/**
  * @brief UART MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    /* USER CODE BEGIN USART1_MspDeInit 0 */

    /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOC, TX_Pin|RX_Pin);

    /* USER CODE BEGIN USART1_MspDeInit 1 */

    /* USER CODE END USART1_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
