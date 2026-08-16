#include "gpio.h"
#include "../../hal/stm32f103.h"


/*
 * GPIOC Pin 13.
 *
 * Commonly connected to the onboard LED
 * on STM32F103C8T6 Blue Pill boards.
 */

#define GPIO_LED_PIN    13U


/*
 * Initialize GPIOC Pin 13 as
 * general-purpose push-pull output.
 */
void GPIO_Init(void)
{
    /*
     * Enable GPIOC clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;


    /*
     * PC13 is configured through CRH.
     *
     * Pins 8-15 use CRH.
     *
     * PC13 occupies bits 20-23.
     */

    GPIOC->CRH &= ~(0xFU << 20);


    /*
     * MODE = 01
     * Output mode, 10 MHz
     *
     * CNF = 00
     * General-purpose push-pull
     */

    GPIOC->CRH |= (0x1U << 20);


    /*
     * Start with PC13 LOW.
     */
    GPIOC->BRR = GPIO_PIN_13;
}


/*
 * Write HIGH or LOW to PC13.
 */
void GPIO_Write(GPIO_State state)
{
    if (state == GPIO_STATE_HIGH)
    {
        GPIOC->BSRR = GPIO_PIN_13;
    }
    else
    {
        GPIOC->BRR = GPIO_PIN_13;
    }
}


/*
 * Toggle PC13.
 */
void GPIO_Toggle(void)
{
    if (GPIOC->ODR & GPIO_PIN_13)
    {
        GPIOC->BRR = GPIO_PIN_13;
    }
    else
    {
        GPIOC->BSRR = GPIO_PIN_13;
    }
}


/*
 * Read PC13.
 */
GPIO_State GPIO_Read(void)
{
    if (GPIOC->IDR & GPIO_PIN_13)
    {
        return GPIO_STATE_HIGH;
    }

    return GPIO_STATE_LOW;
}