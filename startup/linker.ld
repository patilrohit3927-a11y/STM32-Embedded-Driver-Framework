/*
 * STM32F103C8T6 Linker Script
 *
 * Flash: 64 KB
 * RAM:   20 KB
 */

ENTRY(Reset_Handler)

/* Memory layout */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/*
 * Initial stack pointer.
 *
 * Stack grows downward from the end of RAM.
 */
_estack = ORIGIN(RAM) + LENGTH(RAM);


SECTIONS
{
    /*
     * Interrupt vector table and program code.
     *
     * Stored in Flash.
     */
    .text :
    {
        . = ALIGN(4);

        /*
         * Vector table must be at the beginning.
         */
        KEEP(*(.isr_vector))

        /*
         * Program code.
         */
        *(.text)
        *(.text.*)

        /*
         * Read-only data.
         */
        *(.rodata)
        *(.rodata.*)

        . = ALIGN(4);

        _etext = .;
    } > FLASH


    /*
     * Initialized data.
     *
     * The initial values are stored in Flash
     * and copied to RAM by Reset_Handler().
     */
    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data)
        *(.data.*)

        . = ALIGN(4);

        _edata = .;
    } > RAM AT > FLASH


    /*
     * Address in Flash where the initial .data
     * values are stored.
     */
    _sidata = LOADADDR(.data);


    /*
     * Uninitialized data.
     *
     * This section is cleared to zero by
     * Reset_Handler().
     */
    .bss :
    {
        . = ALIGN(4);

        _sbss = .;

        *(.bss)
        *(.bss.*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;
    } > RAM


    /*
     * Remove unnecessary sections.
     */
    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame)
    }
}