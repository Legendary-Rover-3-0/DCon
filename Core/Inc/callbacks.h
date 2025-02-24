#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

/* ### UART ### */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
/* ### CAN ### */
void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

#endif /* __CALLBACKS_H__ */