#ifndef OLED_H
#define OLED_H

#include <stdint.h>

void OLED_Init(void);
void OLED_Clear(void);
void OLED_WriteCommand(uint8_t command);
void OLED_WriteData(uint8_t data);
void OLED_PrintString(const char *text);

#endif