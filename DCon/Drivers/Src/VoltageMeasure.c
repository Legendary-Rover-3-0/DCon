#include "main.h"
#include "string.h"
#include "tim.h"
#include "adc.h"

#include "VoltageMeasure.h"

#define AMOUNT_OF_SAMPLES   (10u)
#define LAST_SAMPLE_IDX     (AMOUNT_OF_SAMPLES - 1u)
#define NO_INITIALIZE       (0xFFu)
#define INITIALIZE          (0u)
#define TIM_INSTANCE        (&htim8)
#define ADC_INSTANCE        (&hadc1)
#define ADC_MAX_VALUE       (4095u)   /* 12-bit ADC */
#define ADC_REF_VOLTAGE     (3300u)   /* Napięcie referencyjne ADC w mV */
#define RESISTOR_UPPER      (100000u) /* Górny rezystor 100kΩ */
#define RESISTOR_LOWER      (15000u)  /* Dolny rezystor 15kΩ */

volatile static uint16_t adcRawVoltage[AMOUNT_OF_SAMPLES] = {0};
static uint8_t isInitialize = NO_INITIALIZE;

void VoltageMeasure_init(void)
{
    HAL_StatusTypeDef retStatus = HAL_OK;

    retStatus |= HAL_TIM_Base_Start(TIM_INSTANCE);
    retStatus |= HAL_ADC_Start_DMA(ADC_INSTANCE, (uint32_t*)adcRawVoltage, AMOUNT_OF_SAMPLES);

    if (retStatus == HAL_OK)
    {
        isInitialize = INITIALIZE;
    }
}

static uint16_t calculateAvarageValue(void)
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

HAL_StatusTypeDef VoltageMeasure_getBatteryVoltage(const VoltageMeasure_Measure_Type measureType, uint16_t* voltage)
{
    uint16_t rawAdc = 0u;
    HAL_StatusTypeDef status = HAL_OK;

    if (isInitialize == INITIALIZE)
    {
        switch(measureType)
        {
            case VOLTAGEMONITOR_AVARAGE_VALUE:
                rawAdc = calculateAvarageValue();
                break;
            case VOLTAGEMONITOR_LAST_VALUE:
                rawAdc = getLastValue();
                break;
        }
        uint32_t adcVoltage = ((uint32_t)rawAdc * ADC_REF_VOLTAGE) / ADC_MAX_VALUE;
        uint32_t batteryVoltage = (adcVoltage * (RESISTOR_UPPER + RESISTOR_LOWER)) / RESISTOR_LOWER;
    
        *voltage = (uint16_t)batteryVoltage;
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}
