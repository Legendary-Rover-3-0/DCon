#ifndef STEPPERMOTOR_DRIVER_H
#define STEPPERMOTOR_DRIVER_H

typedef enum
{
    STEPPER_MOTOR_DISABLE = 0,
    STEPPER_MOTOR_ENABLE = 1,
}StepperMotorEnable_Type;

typedef enum
{
    STEPPER_MOTOR_FORWARD = 0,
    STEPPER_MOTOR_BACKWARD = 1,
}StepperMotorDirection_Type;

typedef enum {
    STEP_FULL = 0,      // Full step
    STEP_HALF,          // Half step
    STEP_QUARTER,       // 1/4 step
    STEP_EIGHTH,        // 1/8 step
    STEP_SIXTEENTH      // 1/16 step
}StepperMotorResolution_Type;

/**
  * @brief Funkcja wł/wy sterownik
  * @param enable   STEPPER_MOTOR_DISABLE - sterownik wyłączony
  *                 STEPPER_MOTOR_ENABLE  - sterownik włączony
  */
void StepperMotor_switchEnable(const StepperMotorEnable_Type enable);

/**
  * @brief Funkcja ustawiająca kierunek obrotów silnika krokowego
  * @param direction    STEPPER_MOTOR_FORWARD   - kierunek do przedni
  *                     STEPPER_MOTOR_BACKWARD  - kierunek wsteczny
  */
void StepperMotor_switchDirection(const StepperMotorDirection_Type direction);

/**
  * @brief Funkcja ustawiająca rozdzielczość kroków sterownika
  * @param resolution   STEP_FULL       - pełny krok
  *                     STEP_HALF       - pół kroku
  *                     STEP_QUARTER    - 1/4 kroku
  *                     STEP_EIGHTH     - 1/8 kroku
  *                     STEP_SIXTEENTH  - 1/16 kroku
  */
void StepperMotor_setStepResolution(const StepperMotorResolution_Type resolution);

/**
  * @brief Funkcja ustawiająca procent wypełnienia sygnału PWM dla sterownika silnika krokowego
  * @param pwmPercent    procent wypełnienia w zakresie od 0 do 100% z krokiem co 1%, funkcja mapuje % w zakresie od 0 do UINT32_MAX
  */
void StepperMotor_setSpeedPercent(const uint8_t pwmPercent);

/**
  * @brief Funkcja inicjalizująca timer generujący PWM, domyślnie wartość wypełnienia jest równa 0%
  */
void StepperMotor_init(void);

#endif