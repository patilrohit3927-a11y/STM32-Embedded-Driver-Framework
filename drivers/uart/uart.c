#include "uart.h"
#include "../../hal/stm32f103.h"
#include "../../interrupt/interrupt.h"


/*
 * ============================================================
 * USART1 CONFIGURATION
 * ============================================================
 *
 * STM32F103C8T6 default clock:
 *
 * HSI = 8 MHz
 *
 * USART1 is connected to APB2.
 *
 * Default APB2 clock = 8 MHz.
 */


/* USART1 peripheral clock */
#define UART_CLOCK_HZ       8000000UL

/* Desired baud rate */
#define UART_BAUD_RATE      115200UL


/*
 * USART BRR calculation.
 *
 * For oversampling by 16:
 *
 * USARTDIV = fCK / (16 * baud)
 *
 * BRR is encoded as:
 *
 * mantissa << 4 | fraction
 *
 * The following integer calculation provides
 * a suitable BRR value for 8 MHz / 115200.
 */
#define UART_BRR_VALUE      0x45U


/*
 * ============================================================
 * RX STATE
 * ============================================================
 */

volatile uint8_t uart_rx_data = 0;
volatile uint8_t uart_rx_ready = 0;


/*
 * ============================================================
 * UART RX HANDLER
 * ============================================================
 */

static void UART_RX_IRQHandler(void)
{
    uint32_t status;

    status = USART1->SR;

    /*
     * RXNE means a byte is available.
     */
    if (status & USART_SR_RXNE)
    {
        /*
         * Reading DR clears RXNE.
         */
        uart_rx_data = (uint8_t)USART1->DR;

        uart_rx_ready = 1U;
    }
}


/*
 * ============================================================
 * USART1 INTERRUPT HANDLER
 * ============================================================
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
     * --------------------------------------------------------
     * PA9 = USART1_TX
     * --------------------------------------------------------
     *
     * MODE = 11
     * Output 50 MHz
     *
     * CNF = 10
     * Alternate-function push-pull
     *
     * Configuration value = 0xB
     */

    GPIOA->CRH &= ~(0xFUL << 4);
    GPIOA->CRH |=  (0xBUL << 4);


    /*
     * --------------------------------------------------------
     * PA10 = USART1_RX
     * --------------------------------------------------------
     *
     * MODE = 00
     * Input
     *
     * CNF = 01
     * Floating input
     *
     * Configuration value = 0x4
     */

    GPIOA->CRH &= ~(0xFUL << 8);
    GPIOA->CRH |=  (0x4UL << 8);


    /*
     * --------------------------------------------------------
     * USART configuration
     * --------------------------------------------------------
     */

    /*
     * Disable USART before configuration.
     */
    USART1->CR1 = 0U;


    /*
     * Configure baud rate.
     *
     * 8 MHz -> 115200 baud
     */
    USART1->BRR = UART_BRR_VALUE;


    /*
     * Enable:
     *
     * UE     = USART enable
     * TE     = transmitter enable
     * RE     = receiver enable
     * RXNEIE = RX interrupt enable
     */
    USART1->CR1 =
        USART_CR1_UE |
        USART_CR1_TE |
        USART_CR1_RE |
        USART_CR1_RXNEIE;


    /*
     * Register interrupt handler.
     */
    Interrupt_RegisterHandler(
        USART1_IRQn,
        UART_RX_IRQHandler
    );


    /*
     * Enable USART1 interrupt in NVIC.
     */
    Interrupt_Enable(USART1_IRQn);
}


/*
 * ============================================================
 * UART SEND BYTE
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
     * Write byte to data register.
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
 * Interrupt-driven reception.
 */

uint8_t UART_ReceiveByte(void)
{
    /*
     * Wait for RX interrupt.
     */
    while (!uart_rx_ready)
    {
    }

    /*
     * Disable/enable ordering is not required here because
     * uart_rx_ready is volatile and only one byte is being
     * buffered.
     */
    uart_rx_ready = 0U;

    return uart_rx_data;
}