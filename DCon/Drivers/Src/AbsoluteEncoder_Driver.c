/**
  ******************************************************************************
  * @file    AbsoluteEncoder_Driver.c
  * @brief   Driver dla enkodera absolutnego LIKA C50MA-BG1-17-10-L2.
  *          PARAMETRY ENKODERA - SSI w kodzie binarnym, rozdzielczość 17bitów, jednoobrotowy 
  ******************************************************************************
  * @author Mateusz Sałęga
  ******************************************************************************
  */

#include "main.h"
#include "spi.h"

#include <stdint.h>

#define ENCODER_MAX_VALUE 131072.0f  // rozdzielczość 17-bitów


uint32_t SPI_ReadSSI(void)
{
    uint8_t receivedData[3]; // Bufor na 3 bajty
    uint32_t u32result = 0;

    // Odczyt danych z enkodera SSI
    HAL_SPI_Receive(&hspi1, receivedData, 3, HAL_MAX_DELAY);

    // Konwersja bajtów na liczbę 17-bitową
    for (int i = 0; i < 3; i++)
    {
        u32result <<= 8;                // Przesunięcie wyniku
        u32result |= receivedData[i];   // Dołączenie kolejnego bajtu
    }

    u32result >>= 7; // Pozostawienie tylko 17 bitów

    return u32result;
}



float ConvertToAngle(uint32_t rawValue)
{
    return ((float)rawValue / ENCODER_MAX_VALUE) * 360.0f;
}



float ReadAngleFromEncoder(void)
{
    uint32_t rawValue = SPI_ReadSSI();
    return ConvertToAngle(rawValue);
}
