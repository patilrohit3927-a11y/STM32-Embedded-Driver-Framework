#include "uart.h"
#include "../../hal/stm32f103.h"


/*
 * USART1 clock.
 *
 * Initial project configuration:
 * 8 MHz.
 */

#define UART_CLOCK_HZ       8000000UL


/*
 * Desired baud rate.
 */

#define UART_BAUD_RATE      115200UL


/*
 * USART baud-rate calculation.
 *
 * USARTDIV = FCK / (16 * Baud)
 *
 * For:
 *
 * FCK   = 8 MHz
 * Baud  = 115200
 *
 * This produces an approximate BRR value.
 */

#define UART_BRR_VALUE      (UART_CLOCK_HZ / UART_BAUD_RATE)


/*
 * Initialize USART1.
 */
void UART_Init(void)
{
    /*
     * Enable GPIOA clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;


    /*
     * Enable USART1 clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;


    /*
     * Configure PA9 as USART1 TX.
     *
     * PA9 is in GPIOA CRH.
     *
     * MODE = 11
     * Output 50 MHz
     *
     * CNF = 10
     * Alternate-function push-pull
     *
     * Configuration = 0xB.
     */

    GPIOA->CRH &= ~(0xFU << 4);
    GPIOA->CRH |=  (0xBU << 4);


    /*
     * Configure PA10 as USART1 RX.
     *
     * MODE = 00
     * Input
     *
     * CNF = 01
     * Floating input
     *
     * Configuration = 0x4.
     */

    GPIOA->CRH &= ~(0xFU << 8);
    GPIOA->CRH |=  (0x4U << 8);


    /*
     * Configure baud rate.
     */
    USART1->BRR = UART_BRR_VALUE;


    /*
     * Enable USART.
     *
     * UE = USART enable
     * TE = transmitter enable
     * RE = receiver enable
     */

    USART1->CR1 =
        USART_CR1_UE |
        USART_CR1_TE |
        USART_CR1_RE;
}


/*
 * Send one byte.
 */
void UART_SendByte(uint8_t data)
{
    /*
     * Wait until transmit data register is empty.
     */
    while (!(USART1->SR & USART_SR_TXE))
    {
    }


    /*
     * Write data to data register.
     */
    USART1->DR = data;
}


/*
 * Send a string.
 */
void UART_SendString(const char *str)
{
    if (str == 0)
    {
        return;
    }


    while (*str != '\0')
    {
        UART_SendByte((uint8_t)*str);

        str++;
    }
}


/*
 * Receive one byte.
 */
uint8_t UART_ReceiveByte(void)
{
    /*
     * Wait for received data.
     */
    while (!(USART1->SR & USART_SR_RXNE))
    {
    }


    return (uint8_t)USART1->DR;
}