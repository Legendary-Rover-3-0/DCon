/**
  ******************************************************************************
  * @file    StepperMotor_Driver.c
  * @brief   Driver dla układu A4988.
  ******************************************************************************
  * @author Mateusz Kudyba
  ******************************************************************************
  */

#include "tim.h"
#include "main.h"
#include "StepperMotor_Driver.h"

#define MAX_PWM_VALUE   ((uint32_t) UINT32_MAX)
#define MAX_PWM_PERCENT ((uint8_t)100u)
#define MIN_PWM_PERCENT ((uint8_t)0u)

/* 
Dokumentacja pinów A4988
| Pin         | Funkcja                                                | Opis                                                        |
|-------------|-------------------------------- |------------------------------------------------------------------------------------|
| **STEP**    | Sterowanie krokiem              | Wykrywa zbocze narastające. Każde zbocze powoduje wykonanie jednego kroku.         |
| **DIR**     | Kierunek obrotu                 | Steruje kierunkiem obrotu silnika: wysoki – jeden kierunek, niski – przeciwny.     |
| **EN**      | Włączenie/wyłączenie sterownika | Wysoki poziom – wyłączenie wyjść (stan wysokiej impedancji), niski – włączenie.    |
| **MS1**     | Ustawienie mikrokroków          | Razem z MS2 i MS3 określa rozdzielczość mikrokroków (patrz: tabela rozdzielczości).|
| **MS2**     | Ustawienie mikrokroków          |                                                                                    |
| **MS3**     | Ustawienie mikrokroków          |                                                                                    |
| **RESET**   | Resetowanie sterownika          | Wysoki – normalna praca, niski – resetuje wewnętrzną logikę sterownika.            |
| **SLEEP**   | Tryb uśpienia                   | Wysoki – normalna praca, niski – tryb uśpienia (oszczędzanie energii).             |

Tabela rozdzielczości mikrokroków
| MS1 | MS2 | MS3 | Rozdzielczość kroku |
|-----|-----|-----|---------------------|
| Low | Low | Low | Pełny krok          |
| High| Low | Low | 1/2 kroku           |
| Low | High| Low | 1/4 kroku           |
| High| High| Low | 1/8 kroku           |
| High| High| High| 1/16 kroku          | 
*/

void StepperMotor_switchEnable(const StepperMotorEnable_Type enable)
{
    GPIO_PinState state = GPIO_PIN_SET;

    state = (enable == STEPPER_MOTOR_ENABLE) ? GPIO_PIN_RESET : GPIO_PIN_SET; /* Jeśli STEPPER_MOTOR_ENABLE sterownik zostanię włączony, w przeciwnym wypadku zostanie wyłączony */
    HAL_GPIO_WritePin(STEPPER_MOTOR_CONTROLLER_ENABLE_GPIO_OUT_GPIO_Port, STEPPER_MOTOR_CONTROLLER_ENABLE_GPIO_OUT_Pin, state);
}

void StepperMotor_switchDirection(const StepperMotorDirection_Type direction)
{
    GPIO_PinState state = GPIO_PIN_SET;

    state = (direction == STEPPER_MOTOR_FORWARD) ? GPIO_PIN_RESET : GPIO_PIN_SET; /* Jeśli STEPPER_MOTOR_FORWARD ustawiony kierunek do przodu, w innym przypadku kierunek wsteczny */
    HAL_GPIO_WritePin(STEPPER_MOTOR_CONTROLLER_DIR_GPIO_OUT_GPIO_Port, STEPPER_MOTOR_CONTROLLER_DIR_GPIO_OUT_Pin, state);
}

void StepperMotor_setStepResolution(const StepperMotorResolution_Type resolution)
{
    GPIO_PinState ms1 = GPIO_PIN_RESET;
    GPIO_PinState ms2 = GPIO_PIN_RESET;
    GPIO_PinState ms3 = GPIO_PIN_RESET;
    /* Funkcja przyjmuje enuma z odpowiedną wartością rozdzielczości i wystawia odpowiednie stany na pinach */
    switch(resolution)
    {
        case STEP_FULL:
            ms1 = GPIO_PIN_RESET;
            ms2 = GPIO_PIN_RESET;
            ms3 = GPIO_PIN_RESET;
            break;
        
        case STEP_HALF:
            ms1 = GPIO_PIN_SET;
            ms2 = GPIO_PIN_RESET;
            ms3 = GPIO_PIN_RESET;
            break;

        case STEP_QUARTER:
            ms1 = GPIO_PIN_RESET;
            ms2 = GPIO_PIN_SET;
            ms3 = GPIO_PIN_RESET;
            break;

        case STEP_EIGHTH:
            ms1 = GPIO_PIN_SET;
            ms2 = GPIO_PIN_SET;
            ms3 = GPIO_PIN_RESET;
            break;

        case STEP_SIXTEENTH:
            ms1 = GPIO_PIN_SET;
            ms2 = GPIO_PIN_SET;
            ms3 = GPIO_PIN_SET;
            break;

        default:
            /* No action, default STEP_FULL mode */
            break;
    }
    HAL_GPIO_WritePin(STEPPER_MOTOR_CONTROLLER_MS1_GPIO_OUT_GPIO_Port, STEPPER_MOTOR_CONTROLLER_MS1_GPIO_OUT_Pin, ms1);
    HAL_GPIO_WritePin(STEPPER_MOTOR_CONTROLLER_MS2_GPIO_OUT_GPIO_Port, STEPPER_MOTOR_CONTROLLER_MS2_GPIO_OUT_Pin, ms2);
    HAL_GPIO_WritePin(STEPPER_MOTOR_CONTROLLER_MS3_GPIO_OUT_GPIO_Port, STEPPER_MOTOR_CONTROLLER_MS3_GPIO_OUT_Pin, ms3);
}

void StepperMotor_setSpeedPercent(const uint8_t pwmPercent)
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
  __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, pwmValue);
}

void StepperMotor_init(void)
{
    StepperMotor_setSpeedPercent(0u); /* Początkowe wypełnienie sygnału PWM 0%*/
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); /* Start timera dla generacji PWM */
}
