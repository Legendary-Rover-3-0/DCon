/**
  ******************************************************************************
  * @file    AbsoluteEncoder_Driver.c
  * @brief   Driver dla enkodera absolutnego LIKA C50MA-BG1-17-10-L2.
  *          PARAMETRY ENKODERA - SSI w kodzie binarnym, rozdzielczość 17bitów, jednoobrotowy
  *          Funkcja "ConvertToAngle" jest wywoływana w przerwaniu od przepełnienia Timera 4 
  *          z częstotliwością 20 Hz.
  ******************************************************************************
  * @author Mateusz Sałęga
  ******************************************************************************
  */

  #include "main.h"
  #include "spi.h"
  
  #include <stdint.h>
  
  #define ENCODER_MAX_VALUE (131072.0f)
  #define DATA_SIZE_BYTE (3u)
  #define SPI_INSTANCE (&hspi1)
  
  uint32_t SPI_ReadSSI(void)
  {
      uint8_t receivedData[DATA_SIZE_BYTE] = {0};
      uint32_t result = 0u;
  
      // Odczyt 3 bajtów (24 bity)
      HAL_SPI_Receive(SPI_INSTANCE, receivedData, DATA_SIZE_BYTE, HAL_MAX_DELAY);
  
      // Konwersja bajtów na liczbę 17-bitową
      result = (receivedData[0] << 16) | (receivedData[1] << 8) | receivedData[2];
  
      // Odrzucenie pierwszego (najstarszego) bitu
      result &= 0x7FFFFF;  // Tylko 23 bity
      result = result >> 6;  // Redukcja do 17 najbardziej znaczących bitów
  
      return result;
  }
  
  // Konwersja liczby binarnej na wartość kątową
  float ConvertToAngle(uint32_t rawValue)
  {
      return ((float)rawValue / ENCODER_MAX_VALUE) * 360.0f;
  }
  
  float ReadAngleFromEncoder(void)
  {
      uint32_t rawValue = SPI_ReadSSI();
      return ConvertToAngle(rawValue);
  }