#include "../drivers/gpio/gpio.h"
#include "../drivers/uart/uart.h"

int main(void)
{
    GPIO_Init();

    UART_Init();

    UART_SendString("STM32 UART INTERRUPT TEST\r\n");

    while (1)
    {
        /*
         * Wait for a byte received by
         * USART1 interrupt.
         */
        uint8_t data = UART_ReceiveByte();

        /*
         * Echo the received byte.
         */
        UART_SendString("IRQ RX: ");
        UART_SendByte(data);
        UART_SendString("\r\n");
    }

    return 0;
}