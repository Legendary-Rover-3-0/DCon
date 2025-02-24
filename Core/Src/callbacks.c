#include "usart.h"
#include "can.h"

#include "DLTuc.h"
#include "Can_Driver.h"
#include "callbacks.h"

#define UART_INSTANCE   ((UART_HandleTypeDef*) &huart1) /* UART do DLT */

/* ### UART ### */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == UART_INSTANCE)
    {
        DLTuc_RawDataReceiveDone(Size);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == UART_INSTANCE)
    {
        DLTuc_MessageTransmitDone();
    }
}

/* ### CAN ### */
void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == CAN_INSTANCE)
    {
        /* Callback od wykrycia braku ruchu na magistrali CAN, ustawinie flagi wymaganej do przejścia w standby */
        Can_Driver_setCanFlag(CAN_STANDBY);
    }
}

void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == CAN_INSTANCE)
    {
        /* Callback od wykrycia ruchu na magistrali CAN, ustawinie flagi wymaganej do przejścia w active */
        Can_Driver_setCanFlag(CAN_ACTIVE);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == CAN_INSTANCE)
    {
        Can_Driver_setLastRxTime();
    }
}