#ifndef VOLTAGEMEASURE_H
#define VOLTAGEMEASURE_H

typedef enum
{
    VOLTAGEMONITOR_AVARAGE_VALUE = 0,
    VOLTAGEMONITOR_LAST_VALUE,
}VoltageMeasure_Measure_Type;

void VoltageMonitor_init(void);

HAL_StatusTypeDef VoltageMeasure_getBatteryVoltage(const VoltageMeasure_Measure_Type measureType, uint16_t* voltage);

#endif