#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

/*
 * Maximum number of IRQs supported
 * by this simple framework.
 */
#define INTERRUPT_MAX_IRQS    64U

/*
 * Interrupt handler function type.
 */
typedef void (*InterruptHandler_t)(void);

/*
 * Initialize interrupt framework.
 */
void Interrupt_Init(void);

/*
 * Register an interrupt handler.
 */
void Interrupt_RegisterHandler(uint32_t irq,
                               InterruptHandler_t handler);

/*
 * Enable an interrupt.
 */
void Interrupt_Enable(uint32_t irq);

/*
 * Disable an interrupt.
 */
void Interrupt_Disable(uint32_t irq);

/*
 * Dispatch an interrupt to the
 * registered handler.
 */
void Interrupt_Dispatch(uint32_t irq);

#endif /* INTERRUPT_H */