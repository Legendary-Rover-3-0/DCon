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

#define MAX_PWM_VALUE       ((uint32_t) UINT32_MAX)
#define MAX_PWM_PERCENT     ((uint8_t)100u)
#define MIN_PWM_PERCENT     ((uint8_t)0u)
#define TIMER_WITH_RCR_REG  (0u)

#if (TIMER_WITH_RCR_REG == 1u)
    #define __FORCE_REGISTERS_VALUE(__HANDLE__)             ((__HANDLE__)->Instance->EGR |= TIM_EGR_UG)
    #define __SET_IMPULSE_AMOUNT(__HANDLE__, __VALUE__)     ((__HANDLE__)->Instance->RCR  = (__VALUE__))
#endif

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
    GPIO_PinState ms[3u] = {GPIO_PIN_RESET};
    GPIO_TypeDef* ports[3u] = {STEPPER_MOTOR_CONTROLLER_MS1_GPIO_OUT_GPIO_Port, STEPPER_MOTOR_CONTROLLER_MS2_GPIO_OUT_GPIO_Port, STEPPER_MOTOR_CONTROLLER_MS3_GPIO_OUT_GPIO_Port};
    uint16_t pins[3u] = {STEPPER_MOTOR_CONTROLLER_MS1_GPIO_OUT_Pin, STEPPER_MOTOR_CONTROLLER_MS2_GPIO_OUT_Pin, STEPPER_MOTOR_CONTROLLER_MS3_GPIO_OUT_Pin};

    /* Funkcja przyjmuje enuma z odpowiedną wartością rozdzielczości i wystawia odpowiednie stany na pinach */
    switch(resolution)
    {
        case STEP_FULL:
            ms[0u] = GPIO_PIN_RESET;
            ms[1u] = GPIO_PIN_RESET;
            ms[2u] = GPIO_PIN_RESET;
            break;
        
        case STEP_HALF:
            ms[0u] = GPIO_PIN_SET;
            ms[1u] = GPIO_PIN_RESET;
            ms[2u] = GPIO_PIN_RESET;
            break;

        case STEP_QUARTER:
            ms[0u] = GPIO_PIN_RESET;
            ms[1u] = GPIO_PIN_SET;
            ms[2u] = GPIO_PIN_RESET;
            break;

        case STEP_EIGHTH:
            ms[0u] = GPIO_PIN_SET;
            ms[1u] = GPIO_PIN_SET;
            ms[2u] = GPIO_PIN_RESET;
            break;

        case STEP_SIXTEENTH:
            ms[0u] = GPIO_PIN_SET;
            ms[1u] = GPIO_PIN_SET;
            ms[2u] = GPIO_PIN_SET;
            break;

        default:
            /* No action, default STEP_FULL mode */
            break;
    }
    
    for(uint8_t i = 0u; i < 3u; i++)
    {
        HAL_GPIO_WritePin(ports[i], pins[i], ms[i]);
    }
}

/**
 * @brief Generuje impulsy sterujące dla sterownika A4988.
 * @details 
 * Funkcja generuje impulsy wymagane przez sterownik A4988, gdzie każde 
 * zbocze narastające sygnału STEP powoduje wykonanie jednego kroku silnika krokowego.
 * Minimalne wymagane czasy impulsu:
 * - Stan wysoki: 1 µs
 * - Stan niski: 1 µs
 *     1us      1us
 *    ┌───┐    ┌───┐    ┌───┐    
 *    │   │    │   │    │   │    
 *   ─┘   └────┘   └────┘   └──  
 *          1 µs     1 µs   (czas impulsów)
 * @param[in] steps Liczba kroków do wykonania (zakres: 0–255).
 */
void StepperMotor_step(const uint8_t steps)
{
  /* timer 80MHz, pcs: 1 cp: 99, impuls około 1.25us */
#if (TIMER_WITH_RCR_REG == 1u)
    uint8_t pulse = 0u;

    __HAL_TIM_DISABLE(&htim2);

    if (steps > 0u)
    {
        pulse = steps - 1u;
    }
    __SET_IMPULSE_AMOUNT(&htim2, pulse);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
    __FORCE_REGISTERS_VALUE(&htim2);
    __HAL_TIM_ENABLE(&htim2);
#else
    /* Do implementacji */
#endif
}

void StepperMotor_init(void)
{
  HAL_TIM_OnePulse_Start(&htim2, TIM_CHANNEL_3);
}
