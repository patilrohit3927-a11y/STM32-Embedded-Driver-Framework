#include <stdint.h>

/*
 * Symbols provided by the linker script.
 */
extern unsigned long _estack;
extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

/*
 * Main application entry point.
 */
int main(void);

/*
 * Default interrupt handler.
 */
void Default_Handler(void)
{
    while (1)
    {
        /*
         * Stay here if an unexpected interrupt occurs.
         */
    }
}

/*
 * Reset handler.
 *
 * This is the first C function executed after reset.
 */
void Reset_Handler(void)
{
    unsigned long *src;
    unsigned long *dst;

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
     * Clear the .bss section.
     */
    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst++ = 0;
    }

    /*
     * Start the application.
     */
    main();

    /*
     * main() should never return.
     */
    while (1)
    {
    }
}


/*
 * Cortex-M3 Vector Table
 *
 * The first entry is the initial stack pointer.
 * The second entry is the reset handler.
 */
__attribute__((section(".isr_vector")))
const void *vector_table[] =
{
    &_estack,

    Reset_Handler,

    /* Cortex-M3 system exceptions */

    Default_Handler,    /* NMI */
    Default_Handler,    /* HardFault */
    Default_Handler,    /* MemManage */
    Default_Handler,    /* BusFault */
    Default_Handler,    /* UsageFault */

    0,
    0,
    0,
    0,

    Default_Handler,    /* SVCall */
    Default_Handler,    /* Debug Monitor */
    0,
    Default_Handler,    /* PendSV */
    Default_Handler     /* SysTick */
};