#include "uart.h"
#include "../../hal/stm32f103.h"


#define UART_CLOCK_HZ   8000000UL
#define UART_BAUD_RATE  115200UL
#define UART_BRR_VALUE  (UART_CLOCK_HZ / UART_BAUD_RATE)


/*
 * Data received by USART1 interrupt.
 */
volatile uint8_t uart_rx_data = 0;


/*
 * Indicates that new data has been received.
 *
 * 0 = no new data
 * 1 = new data available
 */
volatile uint8_t uart_rx_ready = 0;


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
     * PA9 -> USART1_TX
     *
     * Alternate-function push-pull
     * Output speed = 50 MHz
     */
    GPIOA->CRH &= ~(0xFU << 4);
    GPIOA->CRH |=  (0xBU << 4);


    /*
     * PA10 -> USART1_RX
     *
     * Floating input
     */
    GPIOA->CRH &= ~(0xFU << 8);
    GPIOA->CRH |=  (0x4U << 8);


    /*
     * Configure baud rate.
     */
    USART1->BRR = UART_BRR_VALUE;


    /*
     * Enable:
     *
     * UE     = USART
     * TE     = Transmitter
     * RE     = Receiver
     * RXNEIE = RX interrupt
     */
    USART1->CR1 =
        USART_CR1_UE |
        USART_CR1_TE |
        USART_CR1_RE |
        USART_CR1_RXNEIE;


    /*
     * Enable USART1 interrupt in NVIC.
     */
    NVIC_EnableIRQ(USART1_IRQn);
}


/*
 * ============================================================
 * UART TRANSMIT BYTE
 * ============================================================
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
     * Write data to USART.
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
 * ============================================================
 * UART RECEIVE BYTE
 * ============================================================
 *
 * This function now uses the data collected by
 * the USART1 interrupt.
 *
 * It does NOT directly poll RXNE.
 */

uint8_t UART_ReceiveByte(void)
{
    /*
     * Wait until interrupt has received data.
     */
    while (!uart_rx_ready)
    {
    }

    /*
     * Disable interrupts briefly while
     * accessing the shared data.
     */
    uart_rx_ready = 0;

    return uart_rx_data;
}


/*
 * ============================================================
 * USART1 INTERRUPT SERVICE ROUTINE
 * ============================================================
 */

void USART1_IRQHandler(void)
{
    /*
     * Check whether receive data is available.
     */
    if (USART1->SR & USART_SR_RXNE)
    {
        /*
         * Read received byte.
         *
         * Reading DR clears RXNE.
         */
        uart_rx_data = (uint8_t)USART1->DR;

        /*
         * Inform application that
         * new data is available.
         */
        uart_rx_ready = 1;
    }
}