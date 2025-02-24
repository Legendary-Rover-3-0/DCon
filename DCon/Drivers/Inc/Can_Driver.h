#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include "can.h"
#define CAN_INSTANCE    ((CAN_HandleTypeDef*)  &hcan1)  /* CAN */
#define CAN_ACTIVE      ((uint8_t)  0u)
#define CAN_STANDBY     ((uint8_t)  1u)

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

/**
  * @brief Ustawia wewnętrzną flage
  */
void Can_Driver_setCanFlag(const uint8_t flagStatus);

/**
  * @brief Pobiera ostatni czas RX
  */
void Can_Driver_setLastRxTime(void);

#endif /* CAN_DRIVER_H */