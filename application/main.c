#include "../drivers/gpio/gpio.h"
#include "../drivers/uart/uart.h"


int main(void)
{
    /*
     * Initialize GPIO.
     */
    GPIO_Init();


    /*
     * Initialize USART1.
     */
    UART_Init();


    /*
     * Initial message.
     */
    UART_SendString("STM32 UART RX TEST\r\n");


    /*
     * Main application loop.
     */
    while (1)
    {
        uint8_t data;


        /*
         * Wait for one received byte.
         */
        data = UART_ReceiveByte();


        /*
         * Send received byte back.
         *
         * This creates a simple UART
         * echo application.
         */

        UART_SendString("RX: ");

        UART_SendByte(data);

        UART_SendString("\r\n");
    }


    return 0;
}