/*
 * STM32F103C8T6 Linker Script
 *
 * Flash: 64 KB
 * RAM:   20 KB
 */

ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* Initial stack pointer */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /*
     * Code + vector table + read-only data
     */
    .text :
    {
        . = ALIGN(4);

        KEEP(*(.isr_vector))

        *(.text)
        *(.text.*)

        *(.rodata)
        *(.rodata.*)

        . = ALIGN(4);

        _etext = .;
    } > FLASH


    /*
     * Initialized variables
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
     * Source address of .data in Flash
     */
    _sidata = LOADADDR(.data);


    /*
     * Zero-initialized variables
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
     * Discard unnecessary sections
     */
    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame)
    }
}