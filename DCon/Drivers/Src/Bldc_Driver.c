/**
  ******************************************************************************
  * @file    Bldc_Driver.c
  * @brief   Driver dla układu TXS0108EPWR oraz sterownika BLDC 
  ******************************************************************************
  * @author Mateusz Kudyba
  ******************************************************************************
  */

#include "Bldc_Driver.h"
#include "tim.h"

#define MAX_PWM_VALUE   ((uint32_t) UINT32_MAX)
#define MAX_PWM_PERCENT ((uint8_t)100u)
#define MIN_PWM_PERCENT ((uint8_t)0u)

/**
  * @brief Funkcja włączająca konwerter napięć logicznych.
  * 
  */
static void signalConverterOn(void)
{
  HAL_GPIO_WritePin(TXS0108E_ENABLE_GPIO_Port, TXS0108E_ENABLE_Pin, GPIO_PIN_SET);
}

void Bldc_setSpeedPercent(const uint8_t pwmPercent)
{
  uint32_t pwmValue = 0u;
  uint64_t tmpPwmValue = 0u;

  if (pwmPercent >= MAX_PWM_PERCENT)
  {
    pwmValue = MAX_PWM_VALUE;
  }
  else if (pwmPercent <= MIN_PWM_PERCENT)
  {
    pwmValue = 0u;
  }
  else
  {
    tmpPwmValue = ((uint64_t)pwmPercent * MAX_PWM_VALUE) / MAX_PWM_PERCENT;
    pwmValue = (uint32_t)tmpPwmValue;
  }
  __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, pwmValue);
}

uint8_t Bldc_getSpeedPercent(void)
{
  uint32_t pwmValue = 0u;
  uint32_t tmpPercentSpeed = 0u;
  uint8_t percentSpeed = 0u;

  pwmValue = __HAL_TIM_GetCompare(&htim12, TIM_CHANNEL_1);
  tmpPercentSpeed = ((pwmValue * MAX_PWM_PERCENT) * MAX_PWM_VALUE);
  percentSpeed = (uint8_t)tmpPercentSpeed;

  return percentSpeed;
}

void Bldc_break(const uint8_t breakValue)
{
  GPIO_PinState pinState = GPIO_PIN_SET;

  pinState = (breakValue == BREAK) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(BLDC_MOTOR_CONTROLLER_BRK_GPIO_OUT_GPIO_Port, BLDC_MOTOR_CONTROLLER_BRK_GPIO_OUT_Pin, pinState);
}

void Bldc_enable(const bool enable)
{
  GPIO_PinState pinState = GPIO_PIN_SET;

  pinState = (enable == true) ? GPIO_PIN_RESET : GPIO_PIN_SET;
  HAL_GPIO_WritePin(BLDC_MOTOR_CONTROLLER_EN_GPIO_OUT_GPIO_Port, BLDC_MOTOR_CONTROLLER_EN_GPIO_OUT_Pin, pinState);
}

bool Bldc_getAlarmState(void)
{
  GPIO_PinState status = GPIO_PIN_SET;
  bool retStatus = false;

  status = HAL_GPIO_ReadPin(BLDC_MOTOR_CONTROLLER_ALM_GPIO_OUT_GPIO_Port, BLDC_MOTOR_CONTROLLER_ALM_GPIO_OUT_Pin);
  retStatus = (status == GPIO_PIN_SET) ? false : true;
  
  return retStatus;
}

void Bldc_setRotationDirection(const RotationDirection_Type direction)
{
  GPIO_PinState pinState = GPIO_PIN_SET;

  pinState = (direction == BACKWARD) ? GPIO_PIN_RESET : GPIO_PIN_SET;
  HAL_GPIO_WritePin(BLDC_MOTOR_CONTROLLER_F_R_GPIO_OUT_GPIO_Port, BLDC_MOTOR_CONTROLLER_F_R_GPIO_OUT_Pin, pinState);
}

RotationDirection_Type Bldc_getRotationDirection(void)
{
  GPIO_PinState pinState = GPIO_PIN_RESET;

  pinState = HAL_GPIO_ReadPin(BLDC_MOTOR_CONTROLLER_F_R_GPIO_OUT_GPIO_Port, BLDC_MOTOR_CONTROLLER_F_R_GPIO_OUT_Pin);
  return pinState;
}

HAL_StatusTypeDef Bldc_init(void)
{
  HAL_StatusTypeDef status = HAL_OK; /* Status do sprawdzenia poprawności inicjalizacji */

  signalConverterOn(); /* Włączenie konwertera napięć przy inicjalizacji*/

  Bldc_setSpeedPercent(0u); /* Ustawienie zerowego % wypełnienia */
  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1); /* Start timera dla generacji PWM */
  
  // HAL_TIM_IC_Start() /* Start timera dla odczytywania impulsów z pinu speed */
  Bldc_break(NO_BREAK);
  Bldc_setRotationDirection(FORWARD);
  Bldc_enable(true); /* Ustawienie wartości początkowych dla odpowiednik wejść kontrolera */

  return status;
}