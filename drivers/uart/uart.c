#include "uart.h"
#include "../../hal/stm32f103.h"
#include "../../interrupt/interrupt.h"


#define UART_CLOCK_HZ   8000000UL
#define UART_BAUD_RATE  115200UL
#define UART_BRR_VALUE  (UART_CLOCK_HZ / UART_BAUD_RATE)


/*
 * UART receive data.
 */
volatile uint8_t uart_rx_data = 0;


/*
 * Indicates that new data has been received.
 */
volatile uint8_t uart_rx_ready = 0;


/*
 * ============================================================
 * UART RX INTERRUPT HANDLER
 * ============================================================
 *
 * This is the handler registered with the
 * generic interrupt framework.
 */
static void UART_RX_IRQHandler(void)
{
    /*
     * Check RXNE flag.
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
         * Tell the application that
         * new data is available.
         */
        uart_rx_ready = 1;
    }
}


/*
 * ============================================================
 * USART1 ISR
 * ============================================================
 *
 * This function is called directly from the
 * Cortex-M3 interrupt vector table.
 *
 * It forwards the interrupt to the
 * generic interrupt framework.
 */
void USART1_IRQHandler(void)
{
    Interrupt_Dispatch(USART1_IRQn);
}


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
     * Output speed = 50 MHz.
     */
    GPIOA->CRH &= ~(0xFU << 4);
    GPIOA->CRH |=  (0xBU << 4);


    /*
     * PA10 -> USART1_RX
     *
     * Floating input.
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
     * Register UART interrupt handler
     * with generic interrupt framework.
     */
    Interrupt_RegisterHandler(
        USART1_IRQn,
        UART_RX_IRQHandler
    );


    /*
     * Enable USART1 interrupt through
     * generic interrupt framework.
     */
    Interrupt_Enable(USART1_IRQn);
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
 * Reception is interrupt-driven.
 */
uint8_t UART_ReceiveByte(void)
{
    /*
     * Wait until USART1 interrupt
     * receives a byte.
     */
    while (!uart_rx_ready)
    {
    }

    /*
     * Clear ready flag.
     */
    uart_rx_ready = 0;

    return uart_rx_data;
}