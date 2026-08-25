#include "../drivers/gpio/gpio.h"
#include "../drivers/uart/uart.h"
#include "../drivers/spi/spi.h"
#include "../interrupt/interrupt.h"
#include "../config/platform.h"


static void UART_PrintHex(uint8_t value)
{
    const char hex[] = "0123456789ABCDEF";

    UART_SendByte(hex[(value >> 4) & 0x0FU]);
    UART_SendByte(hex[value & 0x0FU]);
}


int main(void)
{
    uint8_t rx;


    /*
     * ========================================================
     * INITIALIZE DRIVER FRAMEWORK
     * ========================================================
     */

    Interrupt_Init();

    GPIO_Init();

    UART_Init();

    SPI_Init();


    /*
     * ========================================================
     * STARTUP MESSAGE
     * ========================================================
     */

    UART_SendString(
        "\r\n"
        "====================================\r\n"
        "   STM32 EMBEDDED DRIVER FRAMEWORK\r\n"
        "====================================\r\n"
    );


    UART_SendString("Platform : ");


#if PLATFORM_RENODE

    UART_SendString("Renode Simulation\r\n");

#else

    UART_SendString("STM32F103C8T6 Hardware\r\n");

#endif


    UART_SendString("\r\n");


    /*
     * ========================================================
     * DRIVER STATUS
     * ========================================================
     */

    UART_SendString("[DRIVERS]\r\n");

    UART_SendString("GPIO       : INITIALIZED\r\n");

    UART_SendString("UART       : INITIALIZED\r\n");

    UART_SendString("SPI        : INITIALIZED\r\n");

    UART_SendString("INTERRUPT  : INITIALIZED\r\n");


    /*
     * ========================================================
     * GPIO DEMONSTRATION
     * ========================================================
     */

    UART_SendString(
        "\r\n[GPIO DEMONSTRATION]\r\n"
    );

    UART_SendString(
        "Toggling onboard LED on PC13\r\n"
    );


    /*
     * ========================================================
     * SPI DEMONSTRATION
     * ========================================================
     */

    UART_SendString(
        "\r\n[SPI DEMONSTRATION]\r\n"
    );


#if PLATFORM_RENODE

    /*
     * Renode simulation.
     *
     * The simulator provides artificial RX data.
     */

    rx = SPI_SimulateTransfer(0x55U);

#else

    /*
     * Physical STM32 hardware.
     *
     * This requires an actual SPI slave device
     * connected to SPI1.
     */

    UART_SendString(
        "Sending 0x55 to SPI slave...\r\n"
    );

    rx = SPI_Transfer(0x55U);

#endif


    UART_SendString("TX: 0x");

    UART_PrintHex(0x55U);


    UART_SendString("\r\nRX: 0x");

    UART_PrintHex(rx);

    UART_SendString("\r\n");


#if PLATFORM_RENODE

    if (rx == 0xAAU)
    {
        UART_SendString("SPI STATUS: PASS\r\n");
    }
    else
    {
        UART_SendString("SPI STATUS: FAIL\r\n");
    }

#else

    UART_SendString(
        "SPI STATUS: TRANSFER COMPLETE\r\n"
    );

#endif


    /*
     * ========================================================
     * UART RX DEMONSTRATION
     * ========================================================
     */

    UART_SendString(
        "\r\n[UART RX DEMONSTRATION]\r\n"
    );

    UART_SendString(
        "Type a character:\r\n"
    );


    /*
     * ========================================================
     * MAIN LOOP
     * ========================================================
     */

    while (1)
    {
        uint8_t data;


        /*
         * Toggle LED every time a character is received.
         */
        data = UART_ReceiveByte();


        UART_SendString("Received: ");

        UART_SendByte(data);

        UART_SendString("\r\n");


        GPIO_Toggle();
    }


    return 0;
}