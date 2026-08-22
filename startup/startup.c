#include <stdint.h>

/*
 * Symbols provided by the linker script.
 */
extern uint32_t _estack;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/*
 * Forward declarations.
 */
void Reset_Handler(void);
void Default_Handler(void);
void USART1_IRQHandler(void);

/*
 * Weak aliases for unused interrupts.
 */
void NMI_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)__attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)   __attribute__((weak, alias("Default_Handler")));

/*
 * Vector table.
 *
 * USART1 is IRQ number 37.
 */
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) =
{
    (void (*)(void))(&_estack),    /* Initial stack pointer */
    Reset_Handler,                 /* Reset */

    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,

    0,
    0,
    0,
    0,

    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,

    /* External IRQs */

    Default_Handler, /* IRQ0  */
    Default_Handler, /* IRQ1  */
    Default_Handler, /* IRQ2  */
    Default_Handler, /* IRQ3  */
    Default_Handler, /* IRQ4  */
    Default_Handler, /* IRQ5  */
    Default_Handler, /* IRQ6  */
    Default_Handler, /* IRQ7  */
    Default_Handler, /* IRQ8  */
    Default_Handler, /* IRQ9  */
    Default_Handler, /* IRQ10 */
    Default_Handler, /* IRQ11 */
    Default_Handler, /* IRQ12 */
    Default_Handler, /* IRQ13 */
    Default_Handler, /* IRQ14 */
    Default_Handler, /* IRQ15 */
    Default_Handler, /* IRQ16 */
    Default_Handler, /* IRQ17 */
    Default_Handler, /* IRQ18 */
    Default_Handler, /* IRQ19 */
    Default_Handler, /* IRQ20 */
    Default_Handler, /* IRQ21 */
    Default_Handler, /* IRQ22 */
    Default_Handler, /* IRQ23 */
    Default_Handler, /* IRQ24 */
    Default_Handler, /* IRQ25 */
    Default_Handler, /* IRQ26 */
    Default_Handler, /* IRQ27 */
    Default_Handler, /* IRQ28 */
    Default_Handler, /* IRQ29 */
    Default_Handler, /* IRQ30 */
    Default_Handler, /* IRQ31 */
    Default_Handler, /* IRQ32 */
    Default_Handler, /* IRQ33 */
    Default_Handler, /* IRQ34 */
    Default_Handler, /* IRQ35 */
    Default_Handler, /* IRQ36 */

    USART1_IRQHandler            /* IRQ37 USART1 */
};


/*
 * Reset handler.
 */
void Reset_Handler(void)
{
    uint32_t *src;
    uint32_t *dst;

    /*
     * Copy .data section.
     */
    src = &_sidata;
    dst = &_sdata;

    while (dst < &_edata)
    {
        *dst++ = *src++;
    }

    /*
     * Clear .bss section.
     */
    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    /*
     * Call application.
     */
    extern int main(void);

    main();

    while (1)
    {
    }
}


/*
 * Default interrupt handler.
 */
void Default_Handler(void)
{
    while (1)
    {
    }
}