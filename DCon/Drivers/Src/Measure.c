#include "tim.h"
#include "adc.h"
#ifdef DLT_ENABLE
    #include "DLTuc.h"
    #define DLT_DEBUG(...) LOGL(DL_INFO, __VA_ARGS__)
#else
    #define DLT_DEBUG(...)
#endif

#include "Measure.h"

#define AMOUNT_OF_SAMPLES       ((uint8_t)  10u)            /* Ilość zbieranych próbek */
#define LAST_SAMPLE_IDX         (AMOUNT_OF_SAMPLES - 1u)    /* Indeks ostatniej próbki */
#define NO_INITIALIZE           ((uint8_t)  0xFFu)          /* Moduł VoltageMonitor nie jest zainicjowany */
#define INITIALIZE              ((uint8_t)  0u)             /* Moduł VoltageMonitor jest zainicjowany poprawnie */
#define ADC_MAX_VALUE           ((uint16_t) 4095u)          /* 12-bit ADC */
#define ADC_REF_VOLTAGE         ((uint16_t) 3300u)          /* Napięcie referencyjne ADC w mV */
#define MAX_BATTERY_VOLTAGE     ((uint16_t) 24000u)         /* Maksymalne napięcie akumulatora */
#define BASE_PRESCALER_VALUE    ((uint16_t) 7999u)          /* Bazowa wartość prescalera */
#define BASE_COUNTER_PERIOD     ((uint16_t) 9999u)          /* Bazowa wartość counter period */
#define MAX_INTERVAL_VALUE      ((ms_t)     5000u)          /* Maksymalna możliwa wartość interwału (5s)*/
#define PERIOD_MULTIPLIER       ((uint16_t) 10u)            /* Mnożnik dla przesunięcia o jedno zero w prawo */

volatile static uint16_t adcRawVoltage[AMOUNT_OF_SAMPLES] = {0}; /*!<  Tablica o rozmiarze AMOUNT_OF_SAMPLES, do której DMA wrzuca odczyt z ADC */
static uint8_t isInitialize = NO_INITIALIZE; /*!<  Flaga do sprawdzenia poprawność inicjalizacji */
static TIM_HandleTypeDef *pTim = NULL; /*!<  Wskaźnik do instancji timera */
static ADC_HandleTypeDef *pAdc = NULL; /*!<  Wskaźnik do instancji ADC */

static void setTimerParam(const uint16_t period)
{
    uint16_t newCounterPeriod = 0u;

    if ((period > 0u) && (period <= MAX_INTERVAL_VALUE)) /* Sprawdzenie czy podana wartość znajduje się w zakresie od 0 do 5000 ms*/
    {
        newCounterPeriod = (period * PERIOD_MULTIPLIER) - 1u;
    }
    else
    {
        newCounterPeriod = BASE_COUNTER_PERIOD; /* 1s */
        DLT_DEBUG("Measure invalid interval value, set default 1s");
    }

    __HAL_TIM_SET_PRESCALER(pTim, BASE_PRESCALER_VALUE); /* Ustawienie wartości dla prescalera */
    __HAL_TIM_SET_AUTORELOAD(pTim, newCounterPeriod);   /* Ustawienie wartości dla counter period */
}

HAL_StatusTypeDef Measure_init(TIM_HandleTypeDef* pTimInstance, ADC_HandleTypeDef* pAdcInstance, const ms_t newMeasureInterval)
{
    HAL_StatusTypeDef retStatus = HAL_OK;

    if ((pTimInstance != NULL) && (pAdcInstance != NULL)) /* Sprawdzenie czy podano odpowiednie wskaźniki do instancji peryferiów */
    {
        pTim = pTimInstance;
        pAdc = pAdcInstance;
        setTimerParam((uint16_t)newMeasureInterval);
        /* Sprawdzenie czy peryferia zostały poprawnie wystartowane */
        retStatus |= HAL_TIM_Base_Start_IT(pTim);
        retStatus |= HAL_ADC_Start_DMA(pAdc, (uint32_t*)adcRawVoltage, AMOUNT_OF_SAMPLES);
    }
    else
    {
        /* Jeśli podano złe wskaźniki do instancji ustaw status na error */
        retStatus = HAL_ERROR;
    }

    if (retStatus == HAL_OK)
    {
        /* Jeśli podano poprawne wskaźniki i funkcje startujące nie zwróciły błędu ustaw flagę isInitialize */
        isInitialize = INITIALIZE;
        DLT_DEBUG("Measure intialized!")
    }
    else
    {
        /* Jeśli coś poszło nie tak zgłośc błąd */
        DLT_DEBUG("Measure initialize error!");
    }
    return retStatus;
}

static uint16_t getAvarageValue(void)
{
    uint32_t value = 0u;
    uint16_t rawAdc[AMOUNT_OF_SAMPLES] = {0};

    for (uint8_t i = 0u; i < AMOUNT_OF_SAMPLES; i++)
    {
        rawAdc[i] += adcRawVoltage[i];
    }
    for (uint8_t i = 0u; i < AMOUNT_OF_SAMPLES; i++)
    {
        value += rawAdc[i];
    }

    value = value / AMOUNT_OF_SAMPLES;

    return (uint16_t)value;
}

static uint16_t getLastValue(void)
{
    return adcRawVoltage[LAST_SAMPLE_IDX];
}

HAL_StatusTypeDef Measure_getBatteryVoltage(const uint8_t measureType, uint16_t* pVoltage)
{
    uint16_t rawAdc = 0u;
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t adcVoltage = 0u;
    uint32_t batteryVoltage = 0u;

    if ((isInitialize == INITIALIZE) && (pVoltage != NULL))
    {
        if (measureType == LAST_VALUE)
        {
            rawAdc = getLastValue();
        }
        else
        {
            rawAdc = getAvarageValue();
        }

        adcVoltage = ((uint32_t)rawAdc * ADC_REF_VOLTAGE) / ADC_MAX_VALUE;
        batteryVoltage = ((adcVoltage * MAX_BATTERY_VOLTAGE) / ADC_REF_VOLTAGE);
        *pVoltage = (uint16_t)batteryVoltage;
    }
    else
    {
        status = HAL_ERROR;
        DLT_DEBUG("Measure error!")
    }
    return status;
}
