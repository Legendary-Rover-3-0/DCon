#ifndef BLDC_DRIVER_H
#define BLDC_DRIVER_H

#include "main.h"
#include "stdbool.h"

#define BREAK       (1u)
#define NO_BREAK    (0u)

typedef enum
{
    FORWARD = 0,
    BACKWARD = 1,
}RotationDirection_Type;

/**
  * @brief Funkcja ustawiająca prędkość silnika
  * @param pwmPercent procent wypełnienia PWM(0-100%)
  * @note Przeliczanie wartości wypełnienia w procentach na wartość uint32 np. 50% wypełnienia będzie odpowiadało 
  * 50% z wartości maksymalnej dla PWm czyli 4294967295
  */
void Bldc_setSpeedPercent(const uint8_t pwmPercent);


/**
  * @brief Funkcja zwracająca prędkość silnika w procentach
  * @return pwmPercent wartość aktualnego PWM
  */
uint8_t Bldc_getSpeedPercent(void);


/**
  * @brief GPIO output, gdy zaciski BRK i COM są rozłączone lub wprowadzany jest sygnał wysoki hamulec silnika zatrzymuje się. Gdy silnik jest zwarciem lub wprowadzany jest
sygnał niski, silnik działa.
    @param breakValue BREAK - na pin zostanie wystawiony stan wysoki, nastąpi hamowanie,
                      NO_BREAK - brak hamowania
  */
void Bldc_break(const uint8_t breakValue);


/**
  * @brief Gdy zacisk EN jest rozłączony z zaciskiem GND lub wprowadzany jest sygnał wysoki, silnik powoli się zatrzymuje.
  * Gdy wprowadzane jest zwarcie lub sygnał niski, silnik działa.
    @param breakValue true - pin w stanie niskim, silnik działa,
                      false - pin w stanie wysokim, silnik nie działa,
  */
void Bldc_enable(const bool enable);


/**
  * @brief Sygnał błędu, 1- poprawna praca, 0- błąd.
    @return bool false -poprawna praca, true -błąd
  */
bool Bldc_getAlarmState(void);


/**
  * @brief Gdy zacisk F/R jest zwarciem z zaciskiem GND lub wprowadzany jest sygnał wysoki, 
  * silnik obraca się do przodu, a gdy wprowadzane jest zwarcie lub sygnał niski, silnik obraca się wstecz.
    @param direction FORWARD - obroty w przód,
                    BACKWARD - obroty w tył,
  */
void Bldc_setRotationDirection(const RotationDirection_Type direction);


/**
  * @brief Zwraca aktualny kierunek działania silnika
    @return FORWARD - obroty w przód, BACKWARD - obroty w tył,
  */
RotationDirection_Type Bldc_getRotationDirection(void);


/**
  * @brief Funkcja inicjalizująca peryferia oraz wartości początkowe.
  * @retval HAL status
  */
HAL_StatusTypeDef Bldc_init(void);

#endif