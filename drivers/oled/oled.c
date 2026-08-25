#include "oled.h"
#include "../spi/spi.h"

/*
 * OLED command/data interface.
 *
 * DC, CS and RESET pins will be connected to GPIO pins
 * when the hardware is connected.
 */

#define OLED_WIDTH   128
#define OLED_HEIGHT   64

static void OLED_Delay(void)
{
    volatile uint32_t count;

    for (count = 0; count < 10000; count++)
    {
        __asm volatile ("nop");
    }
}

void OLED_WriteCommand(uint8_t command)
{
    /*
     * DC = 0
     *
     * CS = 0
     *
     * SPI transfer
     *
     * CS = 1
     */

    SPI_Transfer(command);
}

void OLED_WriteData(uint8_t data)
{
    /*
     * DC = 1
     *
     * CS = 0
     *
     * SPI transfer
     *
     * CS = 1
     */

    SPI_Transfer(data);
}

void OLED_Init(void)
{
    SPI_Init();

    OLED_Delay();

    /*
     * OLED initialization sequence will be added
     * after confirming the exact OLED controller.
     */
}

void OLED_Clear(void)
{
    uint16_t i;

    for (i = 0; i < OLED_WIDTH * OLED_HEIGHT / 8; i++)
    {
        OLED_WriteData(0x00);
    }
}

void OLED_PrintString(const char *text)
{
    /*
     * Character/font rendering will be added next.
     */

    while (*text)
    {
        /*
         * Temporary demonstration:
         * send character byte over SPI.
         */
        OLED_WriteData((uint8_t)*text);

        text++;
    }
}