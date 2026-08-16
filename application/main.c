#include "../drivers/gpio/gpio.h"
#include "../drivers/uart/uart.h"

int main(void)
{
    GPIO_Init();

    GPIO_Write(GPIO_STATE_HIGH);

    UART_Init();

    UART_SendString("Hello from STM32 UART\r\n");

    while (1)
    {
    }

    return 0;
}