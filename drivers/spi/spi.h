#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void SPI_Init(void);

uint8_t SPI_Transfer(uint8_t data);

void SPI_SendByte(uint8_t data);

uint8_t SPI_ReceiveByte(void);

uint8_t SPI_SimulateTransfer(uint8_t data);

#endif /* SPI_H */