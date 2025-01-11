#include "stdint.h"
#include "DCon_App.h"
#include "Bldc_Driver.h"

/* Przykładowa implementacja */
typedef enum
{
    DCON_APP_START = 0,
    DCON_APP_RUN,
    DCON_APP_STOP,
    DCON_APP_IDLE,
}DCon_AppState_Type; /* Stany działania aplikacji */

static DCon_AppState_Type appState = DCON_APP_START; /* Zmienna odpowiadajaca za stan */

void DCon_main(void)
{
    static bool alarmState = false; /* Zmienna odpowiadająca za stan alarmu, static sprawia, że jest jest zapamiętywana między wywołaniami funkcji */
    HAL_StatusTypeDef initState = HAL_OK; /* lokalna zmienna odpowiadająca za stan initu */

    switch(appState)
    {
        case DCON_APP_START:
            initState = Bldc_init();
            if (initState == HAL_OK)
            {
                appState = DCON_APP_RUN;
            }
        break;

        case DCON_APP_RUN:
            Bldc_setSpeedPercent(50u);
            alarmState = Bldc_getAlarmState();
            if (alarmState == true)
            {
                appState = DCON_APP_STOP;
            }
        break;

        case DCON_APP_STOP:
            Bldc_enable(false);
            appState = DCON_APP_IDLE;
        break;

        case DCON_APP_IDLE:
            alarmState = Bldc_getAlarmState();
            if (alarmState == false)
            {
                appState = DCON_APP_START;
            }
        break;
    }
}