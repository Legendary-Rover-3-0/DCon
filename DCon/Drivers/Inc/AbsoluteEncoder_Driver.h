#ifndef ABSOLUTEENCODER_H
#define ABSOLUTEENCODER_H
#include <stdint.h>

uint32_t GrayToBinary(uint32_t num);
uint32_t SPI_ReadSSI(void);
float ConvertToAngle(uint32_t rawValue);
float ReadAngleFromEncoder(void);
#endif