#include "gpio.h"
#include "../../hal/stm32f103.h"


/*
 * ============================================================
 * STM32F103C8T6 GPIO DRIVER
 * ============================================================
 *
 * LED:
 *
 * Blue Pill onboard LED:
 *     PC13
 *
 * The onboard LED is normally ACTIVE LOW.
 *
 * Therefore:
 *
 * GPIO_STATE_LOW  -> LED ON
 * GPIO_STATE_HIGH -> LED OFF
 */


/* Onboard LED */
#define GPIO_LED_PIN       GPIO_PIN_13


/* ============================================================
 * GPIO INITIALIZATION
 * ============================================================ */

void GPIO_Init(void)
{
    /*
     * Enable GPIOC clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /*
     * Configure PC13.
     *
     * PC13 is in CRH.
     *
     * Pin 13:
     *
     * bits 20-23
     *
     * MODE = 01
     * CNF  = 00
     *
     * Result:
     *
     * General purpose push-pull output
     * 10 MHz
     */

    GPIOC->CRH &= ~(0xFUL << 20);
    GPIOC->CRH |=  (0x1UL << 20);

    /*
     * Blue Pill LED is active LOW.
     *
     * Drive HIGH initially so LED is OFF.
     */
    GPIOC->BSRR = GPIO_LED_PIN;
}


/* ============================================================
 * GPIO WRITE
 * ============================================================ */

void GPIO_Write(GPIO_State state)
{
    if (state == GPIO_STATE_HIGH)
    {
        /*
         * PC13 HIGH
         *
         * LED OFF on Blue Pill.
         */
        GPIOC->BSRR = GPIO_LED_PIN;
    }
    else
    {
        /*
         * PC13 LOW
         *
         * LED ON on Blue Pill.
         */
        GPIOC->BRR = GPIO_LED_PIN;
    }
}


/* ============================================================
 * GPIO TOGGLE
 * ============================================================ */

void GPIO_Toggle(void)
{
    if (GPIOC->ODR & GPIO_LED_PIN)
    {
        /*
         * Currently HIGH.
         *
         * Drive LOW.
         */
        GPIOC->BRR = GPIO_LED_PIN;
    }
    else
    {
        /*
         * Currently LOW.
         *
         * Drive HIGH.
         */
        GPIOC->BSRR = GPIO_LED_PIN;
    }
}


/* ============================================================
 * GPIO READ
 * ============================================================ */

GPIO_State GPIO_Read(void)
{
    if (GPIOC->IDR & GPIO_LED_PIN)
    {
        return GPIO_STATE_HIGH;
    }

    return GPIO_STATE_LOW;
}