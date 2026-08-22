#include "interrupt.h"
#include "../hal/stm32f103.h"


/*
 * Interrupt handler table.
 */
static InterruptHandler_t
interrupt_handlers[INTERRUPT_MAX_IRQS];


/*
 * Initialize interrupt framework.
 */
void Interrupt_Init(void)
{
    uint32_t i;

    for (i = 0; i < INTERRUPT_MAX_IRQS; i++)
    {
        interrupt_handlers[i] = 0;
    }
}


/*
 * Register interrupt handler.
 */
void Interrupt_RegisterHandler(uint32_t irq,
                               InterruptHandler_t handler)
{
    if (irq >= INTERRUPT_MAX_IRQS)
    {
        return;
    }

    interrupt_handlers[irq] = handler;
}


/*
 * Enable interrupt.
 */
void Interrupt_Enable(uint32_t irq)
{
    if (irq >= INTERRUPT_MAX_IRQS)
    {
        return;
    }

    NVIC_EnableIRQ(irq);
}


/*
 * Disable interrupt.
 */
void Interrupt_Disable(uint32_t irq)
{
    /*
     * NVIC ICER registers.
     */
    volatile uint32_t *NVIC_ICER0 =
        (volatile uint32_t *)0xE000E180UL;

    volatile uint32_t *NVIC_ICER1 =
        (volatile uint32_t *)0xE000E184UL;

    if (irq < 32U)
    {
        *NVIC_ICER0 = (1UL << irq);
    }
    else
    {
        *NVIC_ICER1 = (1UL << (irq - 32U));
    }
}


/*
 * Dispatch interrupt.
 */
void Interrupt_Dispatch(uint32_t irq)
{
    if (irq >= INTERRUPT_MAX_IRQS)
    {
        return;
    }

    if (interrupt_handlers[irq] != 0)
    {
        interrupt_handlers[irq]();
    }
}