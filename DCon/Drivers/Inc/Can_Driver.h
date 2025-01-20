#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include "can.h"

#define CAN_INSTANCE        ((CAN_HandleTypeDef*) &hcan1)

/**
  * @brief Funcka inicjalizująca driver CAN
  */
HAL_StatusTypeDef Can_Driver_init(void);

/**
  * @brief Wybudzenie trancivera przed wysłaniem wiadomości
  */
void Can_Driver_wakeup(void);

/**
  * @brief Główna funkcja zarządzająca hardwardwerową obsłguą trancivera
  */
void Can_Driver_stateMenager(void);

void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

#endif /* CAN_DRIVER_H */