#include "uart.h"
#include "../../hal/stm32f103.h"


/*
 * USART1 clock frequency.
 */
#define UART_CLOCK_HZ       8000000UL


/*
 * UART baud rate.
 */
#define UART_BAUD_RATE      115200UL


/*
 * Baud rate register value.
 */
#define UART_BRR_VALUE      (UART_CLOCK_HZ / UART_BAUD_RATE)


/*
 * ============================================================
 * UART INITIALIZATION
 * ============================================================
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
     * --------------------------------------------------------
     * PA9 -> USART1_TX
     * --------------------------------------------------------
     *
     * MODE = 11
     * Output mode, 50 MHz
     *
     * CNF = 10
     * Alternate-function push-pull
     *
     * Configuration = 0xB
     */

    GPIOA->CRH &= ~(0xFU << 4);
    GPIOA->CRH |=  (0xBU << 4);


    /*
     * --------------------------------------------------------
     * PA10 -> USART1_RX
     * --------------------------------------------------------
     *
     * MODE = 00
     * Input
     *
     * CNF = 01
     * Floating input
     *
     * Configuration = 0x4
     */

    GPIOA->CRH &= ~(0xFU << 8);
    GPIOA->CRH |=  (0x4U << 8);


    /*
     * Configure baud rate.
     */
    USART1->BRR = UART_BRR_VALUE;


    /*
     * Enable USART1.
     *
     * UE = USART enable
     * TE = Transmitter enable
     * RE = Receiver enable
     */

    USART1->CR1 =
        USART_CR1_UE |
        USART_CR1_TE |
        USART_CR1_RE;
}


/*
 * ============================================================
 * UART TRANSMIT
 * ============================================================
 */

void UART_SendByte(uint8_t data)
{
    /*
     * Wait until transmit data register
     * is empty.
     */
    while (!(USART1->SR & USART_SR_TXE))
    {
    }


    /*
     * Write byte to USART data register.
     */
    USART1->DR = data;
}


/*
 * ============================================================
 * UART SEND STRING
 * ============================================================
 */

void UART_SendString(const char *str)
{
    /*
     * Protect against NULL pointer.
     */
    if (str == 0)
    {
        return;
    }


    /*
     * Send characters until
     * NULL terminator.
     */
    while (*str != '\0')
    {
        UART_SendByte((uint8_t)*str);

        str++;
    }
}


/*
 * ============================================================
 * UART RECEIVE
 * ============================================================
 */

uint8_t UART_ReceiveByte(void)
{
    /*
     * Wait until receive data
     * register is not empty.
     *
     * RXNE = 1 means data is available.
     */
    while (!(USART1->SR & USART_SR_RXNE))
    {
    }


    /*
     * Read received byte.
     *
     * Reading DR clears the RXNE
     * condition in normal USART operation.
     */
    return (uint8_t)USART1->DR;
}