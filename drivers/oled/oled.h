#ifndef OLED_H
#define OLED_H

#include <stdint.h>

#define OLED_WIDTH       128U
#define OLED_HEIGHT       64U
#define OLED_I2C_ADDRESS 0x3CU

void OLED_Init(void);

void OLED_Clear(void);

void OLED_Update(void);

void OLED_SetPixel(uint8_t x, uint8_t y, uint8_t state);

void OLED_WriteChar(uint8_t x, uint8_t y, char character);

void OLED_WriteString(uint8_t x, uint8_t y, const char *string);

#endif /* OLED_H */