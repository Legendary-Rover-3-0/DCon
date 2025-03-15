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

/* PARAMETRY DO TESTÓW DO USTAWIENIA */
/*  
    1. Ustawienia dla timera w trybie OnePulse: Timer 80MHz, pcs: 1 cp: 99, co daje czas 1 impulsu około 1.25us
    1. Usatwić piny GPIO podstawiając definicje poniżej.
    2. Wtwołać inita w main.
    3. Wywołać funkcję parsującą w while maina.
*/

#define ENABLE_PORT         (STEPPER_MOTOR_CONTROLLER_ENABLE_GPIO_OUT_GPIO_Port) /* Do podmiany */
#define ENABLE_PIN          (STEPPER_MOTOR_CONTROLLER_ENABLE_GPIO_OUT_Pin) /* Do podmiany */
#define DIR_PORT            (STEPPER_MOTOR_CONTROLLER_DIR_GPIO_OUT_GPIO_Port) /* Do podmiany */
#define DIR_PIN             (STEPPER_MOTOR_CONTROLLER_DIR_GPIO_OUT_Pin) /* Do podmiany */
#define MS1_PORT            (STEPPER_MOTOR_CONTROLLER_MS1_GPIO_OUT_GPIO_Port) /* Do podmiany */
#define MS2_PORT            (STEPPER_MOTOR_CONTROLLER_MS2_GPIO_OUT_GPIO_Port) /* Do podmiany */
#define MS3_PORT            (STEPPER_MOTOR_CONTROLLER_MS3_GPIO_OUT_GPIO_Port) /* Do podmiany */
#define MS1_PIN             (STEPPER_MOTOR_CONTROLLER_MS1_GPIO_OUT_Pin) /* Do podmiany */
#define MS2_PIN             (STEPPER_MOTOR_CONTROLLER_MS2_GPIO_OUT_Pin) /* Do podmiany */
#define MS3_PIN             (STEPPER_MOTOR_CONTROLLER_MS3_GPIO_OUT_Pin) /* Do podmiany */
#define TIMER_INSTANCE      (&htim2)
#define ONEPULSE_CHANNEL    (TIM_CHANNEL_3)
/* END */

#define MAX_PWM_VALUE       ((uint32_t) UINT32_MAX)
#define MAX_PWM_PERCENT     ((uint8_t)100u)
#define MIN_PWM_PERCENT     ((uint8_t)0u)
#define TIMER_WITH_RCR_REG  (1u)

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
    HAL_GPIO_WritePin(ENABLE_PORT, ENABLE_PIN, state);
}

void StepperMotor_switchDirection(const StepperMotorDirection_Type direction)
{
    GPIO_PinState state = GPIO_PIN_SET;

    state = (direction == STEPPER_MOTOR_FORWARD) ? GPIO_PIN_RESET : GPIO_PIN_SET; /* Jeśli STEPPER_MOTOR_FORWARD ustawiony kierunek do przodu, w innym przypadku kierunek wsteczny */
    HAL_GPIO_WritePin(DIR_PORT, DIR_PIN, state);
}

void StepperMotor_setStepResolution(const StepperMotorResolution_Type resolution)
{
    GPIO_PinState ms[3u] = {GPIO_PIN_RESET};
    GPIO_TypeDef* ports[3u] = {MS1_PORT, MS2_PORT, MS3_PORT};
    uint16_t pins[3u] = {MS1_PIN, MS2_PIN, MS3_PIN};

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
    
    for (uint8_t i = 0u; i < 3u; i++)
    {
        HAL_GPIO_WritePin(ports[i], pins[i], ms[i]);
    }
}

void StepperMotor_step(const uint8_t steps)
{
  /* timer 80MHz, pcs: 1 cp: 99, impuls około 1.25us */
#if (TIMER_WITH_RCR_REG == 1u)
    uint8_t pulse = 0u;

    __HAL_TIM_DISABLE(TIMER_INSTANCE);

    if (steps > 0u)
    {
        pulse = steps - 1u;
    }
    __SET_IMPULSE_AMOUNT(TIMER_INSTANCE, pulse);
    __HAL_TIM_SET_COUNTER(TIMER_INSTANCE, 0);
    __HAL_TIM_CLEAR_FLAG(TIMER_INSTANCE, TIM_FLAG_UPDATE);
    __FORCE_REGISTERS_VALUE(TIMER_INSTANCE);
    __HAL_TIM_ENABLE(TIMER_INSTANCE);
#else
    /* Do implementacji */
#endif
}

void StepperMotor_init(void)
{
    HAL_TIM_OnePulse_Start(TIMER_INSTANCE, ONEPULSE_CHANNEL);
}
