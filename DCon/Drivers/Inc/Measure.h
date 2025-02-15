#ifndef MEASURE_H
#define MEASURE_H

#define AVARAGE_VALUE   ((uint8_t) 0u) /* Weź wartość średnią z N pomiarów */
#define LAST_VALUE      ((uint8_t) 1u) /* Weź wartość ostatnią z N pomiarów */

typedef uint16_t ms_t; /* Typ dla ustawienia interwału konwersji ADC w mili sekundach */

/**
 * @brief Funckja inicjalizująca moduł Measure -pomiar napięcią na akumulatorze. Funkcja wykorzystuje przepełnienie timera
 * do wywołania konwersji ADC z DMA.
 * @param[inout]    pTimInstance wskaźnik do instancji timera
 * @param[inout]    pAdcInstance wskaźnik do instancji ADC
 * @param[in]       newMeasureInterval interwał zlecania konwersji ADC podawany w ms. Wartości w zakresie od 1ms do 5000ms.
 * @return          HAL_StatusTypeDef -status operacji
 *                      HAL_OK      -poprawna inicjalizacja
 *                      HAL_ERROR   -błąd inicjalizacja
 */
HAL_StatusTypeDef Measure_init(TIM_HandleTypeDef* pTimInstance, ADC_HandleTypeDef* pAdcInstance, const ms_t newMeasureInterval);

/**
 * @brief Funkcja zwraca poprzez referencję wartość napięcia w mV.
 * @param[in]       measureType typ pomiaru
 *                      AVARAGE_VALUE -zwróć wartość średnią
 *                      LAST_VALUE    -zwróć ostatnią wartość
 * @param[inout]    pVoltage wskaźnik do zmiennej pod której adres ma zostać wpisane napięcie
 * @return          HAL_StatusTypeDef -status operacji
 *                      HAL_OK      -poprawny pomiar
 *                      HAL_ERROR   -błąd pomiaru
 */
HAL_StatusTypeDef Measure_getBatteryVoltage(const uint8_t measureType, uint16_t* pVoltage);

#endif