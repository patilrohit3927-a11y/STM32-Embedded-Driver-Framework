#include <stdint.h>

/* ============================================================
 * Linker symbols
 * ============================================================ */

extern uint32_t _estack;

extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;

extern uint32_t _sbss;
extern uint32_t _ebss;


/* ============================================================
 * Function declarations
 * ============================================================ */

void Reset_Handler(void);
void Default_Handler(void);

void USART1_IRQHandler(void);


/* ============================================================
 * Weak interrupt handlers
 * ============================================================ */

void NMI_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void HardFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void MemManage_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void BusFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void UsageFault_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void SVC_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void DebugMon_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void PendSV_Handler(void)
    __attribute__((weak, alias("Default_Handler")));

void SysTick_Handler(void)
    __attribute__((weak, alias("Default_Handler")));


/* ============================================================
 * Interrupt Vector Table
 *
 * Cortex-M3 core exceptions:
 * Vector 0  -> Initial Stack Pointer
 * Vector 1  -> Reset
 *
 * External IRQ starts from vector 16.
 *
 * USART1 IRQ = 37
 * Therefore:
 *
 * Vector index = 16 + 37 = 53
 * ============================================================ */

__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) =
{
    /* 0  */ (void (*)(void))(&_estack),
    /* 1  */ Reset_Handler,

    /* Cortex-M3 exceptions */

    /* 2  */ NMI_Handler,
    /* 3  */ HardFault_Handler,
    /* 4  */ MemManage_Handler,
    /* 5  */ BusFault_Handler,
    /* 6  */ UsageFault_Handler,

    /* 7  */ 0,
    /* 8  */ 0,
    /* 9  */ 0,
    /* 10 */ 0,

    /* 11 */ SVC_Handler,
    /* 12 */ DebugMon_Handler,
    /* 13 */ 0,
    /* 14 */ PendSV_Handler,
    /* 15 */ SysTick_Handler,


    /* ========================================================
     * External Interrupts
     * ======================================================== */

    /* IRQ 0  */
    Default_Handler,

    /* IRQ 1  */
    Default_Handler,

    /* IRQ 2  */
    Default_Handler,

    /* IRQ 3  */
    Default_Handler,

    /* IRQ 4  */
    Default_Handler,

    /* IRQ 5  */
    Default_Handler,

    /* IRQ 6  */
    Default_Handler,

    /* IRQ 7  */
    Default_Handler,

    /* IRQ 8  */
    Default_Handler,

    /* IRQ 9  */
    Default_Handler,

    /* IRQ 10 */
    Default_Handler,

    /* IRQ 11 */
    Default_Handler,

    /* IRQ 12 */
    Default_Handler,

    /* IRQ 13 */
    Default_Handler,

    /* IRQ 14 */
    Default_Handler,

    /* IRQ 15 */
    Default_Handler,

    /* IRQ 16 */
    Default_Handler,

    /* IRQ 17 */
    Default_Handler,

    /* IRQ 18 */
    Default_Handler,

    /* IRQ 19 */
    Default_Handler,

    /* IRQ 20 */
    Default_Handler,

    /* IRQ 21 */
    Default_Handler,

    /* IRQ 22 */
    Default_Handler,

    /* IRQ 23 */
    Default_Handler,

    /* IRQ 24 */
    Default_Handler,

    /* IRQ 25 */
    Default_Handler,

    /* IRQ 26 */
    Default_Handler,

    /* IRQ 27 */
    Default_Handler,

    /* IRQ 28 */
    Default_Handler,

    /* IRQ 29 */
    Default_Handler,

    /* IRQ 30 */
    Default_Handler,

    /* IRQ 31 */
    Default_Handler,

    /* IRQ 32 */
    Default_Handler,

    /* IRQ 33 */
    Default_Handler,

    /* IRQ 34 */
    Default_Handler,

    /* IRQ 35 */
    Default_Handler,

    /* IRQ 36 */
    Default_Handler,

    /* IRQ 37 - USART1 */
    USART1_IRQHandler
};


/* ============================================================
 * Reset Handler
 * ============================================================ */

void Reset_Handler(void)
{
    uint32_t *src;
    uint32_t *dst;


    /*
     * Copy initialized data from Flash to RAM.
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
     * Start application.
     */
    extern int main(void);

    main();


    /*
     * main() should never return.
     */
    while (1)
    {
    }
}


/* ============================================================
 * Default Interrupt Handler
 * ============================================================ */

void Default_Handler(void)
{
    while (1)
    {
    }
}