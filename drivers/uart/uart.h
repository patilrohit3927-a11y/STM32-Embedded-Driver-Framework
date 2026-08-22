#ifndef UART_H
#define UART_H

#include <stdint.h>


/*
 * Initialize USART1.
 */
void UART_Init(void);


/*
 * Transmit one byte.
 */
void UART_SendByte(uint8_t data);


/*
 * Transmit a string.
 */
void UART_SendString(const char *str);


/*
 * Receive one byte.
 *
 * This function waits until
 * a byte is available.
 */
uint8_t UART_ReceiveByte(void);


#endif /* UART_H */