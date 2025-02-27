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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STEPPER_MOTOR_CONTROLLER_ENABLE_GPIO_OUT_Pin GPIO_PIN_0
#define STEPPER_MOTOR_CONTROLLER_ENABLE_GPIO_OUT_GPIO_Port GPIOC
#define STEPPER_MOTOR_CONTROLLER_MS1_GPIO_OUT_Pin GPIO_PIN_1
#define STEPPER_MOTOR_CONTROLLER_MS1_GPIO_OUT_GPIO_Port GPIOC
#define STEPPER_MOTOR_CONTROLLER_MS2_GPIO_OUT_Pin GPIO_PIN_2
#define STEPPER_MOTOR_CONTROLLER_MS2_GPIO_OUT_GPIO_Port GPIOC
#define STEPPER_MOTOR_CONTROLLER_MS3_GPIO_OUT_Pin GPIO_PIN_3
#define STEPPER_MOTOR_CONTROLLER_MS3_GPIO_OUT_GPIO_Port GPIOC
#define STEPPER_MOTOR_CONTROLLER_RESET_GPIO_OUT_Pin GPIO_PIN_0
#define STEPPER_MOTOR_CONTROLLER_RESET_GPIO_OUT_GPIO_Port GPIOA
#define STEPPER_MOTOR_CONTROLLER_SLEEP_GPIO_OUT_Pin GPIO_PIN_1
#define STEPPER_MOTOR_CONTROLLER_SLEEP_GPIO_OUT_GPIO_Port GPIOA
#define STEPPER_MOTOR_CONTROLLER_STEP_PWM_Pin GPIO_PIN_2
#define STEPPER_MOTOR_CONTROLLER_STEP_PWM_GPIO_Port GPIOA
#define STEPPER_MOTOR_CONTROLLER_DIR_GPIO_OUT_Pin GPIO_PIN_3
#define STEPPER_MOTOR_CONTROLLER_DIR_GPIO_OUT_GPIO_Port GPIOA
#define ABSOLUTE_ENCODER_D_ATA_OUT_SPI_Pin GPIO_PIN_5
#define ABSOLUTE_ENCODER_D_ATA_OUT_SPI_GPIO_Port GPIOA
#define ABSOLUTE_ENCODER_CL_K_IN_SPI_Pin GPIO_PIN_7
#define ABSOLUTE_ENCODER_CL_K_IN_SPI_GPIO_Port GPIOA
#define ADC_VBATT_Pin GPIO_PIN_0
#define ADC_VBATT_GPIO_Port GPIOB
#define BLDC_MOTOR_CONTROLLER_SV_PWM_Pin GPIO_PIN_14
#define BLDC_MOTOR_CONTROLLER_SV_PWM_GPIO_Port GPIOB
#define BLDC_MOTOR_CONTROLLER_F_R_GPIO_OUT_Pin GPIO_PIN_15
#define BLDC_MOTOR_CONTROLLER_F_R_GPIO_OUT_GPIO_Port GPIOB
#define BLDC_MOTOR_CONTROLLER_EN_GPIO_OUT_Pin GPIO_PIN_6
#define BLDC_MOTOR_CONTROLLER_EN_GPIO_OUT_GPIO_Port GPIOC
#define BLDC_MOTOR_CONTROLLER_BRK_GPIO_OUT_Pin GPIO_PIN_7
#define BLDC_MOTOR_CONTROLLER_BRK_GPIO_OUT_GPIO_Port GPIOC
#define BLDC_MOTOR_CONTROLLER_PG_GPIO_OUT_Pin GPIO_PIN_8
#define BLDC_MOTOR_CONTROLLER_PG_GPIO_OUT_GPIO_Port GPIOC
#define BLDC_MOTOR_CONTROLLER_ALM_GPIO_OUT_Pin GPIO_PIN_9
#define BLDC_MOTOR_CONTROLLER_ALM_GPIO_OUT_GPIO_Port GPIOC
#define TXS0108E_ENABLE_Pin GPIO_PIN_8
#define TXS0108E_ENABLE_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_12
#define LED_GPIO_Port GPIOC
#define INCREMENTAL_ENCODER__CH_A_PWM_Pin GPIO_PIN_4
#define INCREMENTAL_ENCODER__CH_A_PWM_GPIO_Port GPIOB
#define INCREMENTAL_ENCODER__CH_B_PWM_Pin GPIO_PIN_5
#define INCREMENTAL_ENCODER__CH_B_PWM_GPIO_Port GPIOB
#define CAN_RS_Pin GPIO_PIN_7
#define CAN_RS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
