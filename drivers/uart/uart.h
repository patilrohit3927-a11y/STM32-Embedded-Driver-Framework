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
 * Data is received through the USART1
 * interrupt handler.
 */
uint8_t UART_ReceiveByte(void);


/*
 * USART1 interrupt handler.
 */
void USART1_IRQHandler(void);


#endif /* UART_H */