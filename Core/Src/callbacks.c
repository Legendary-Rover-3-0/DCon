#include "usart.h"
#include "can.h"

#include "DLTuc.h"
#include "Can_Driver.h"
#include "callbacks.h"
#include "RingBuffer.h"
#include "parse.h"


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

/* ### USB ### */
void CDC_ReveiveCallback(uint8_t *Buffer, uint8_t Length)
{
    if(Length > 0)
    {
        //CDC_Transmit_FS(Buffer, Length); // Echo danych

        for(uint8_t i = 0; i < Length; i++)
        {
            if(RB_OK == RB_Write(&ReceiveBuffer, Buffer[i]))
            {
                if(Buffer[i] == ENDLINE)
                {
                    ReceivedLines++;
                }
            }
        }
    }
}

// void CDC_TransmitCallback(uint8_t *Buffer, uint8_t Length)
// {
//     /* Wysyłanie danych przez USB */
//     UNUSED(Buffer);
//     UNUSED
//     (Length);
// }

