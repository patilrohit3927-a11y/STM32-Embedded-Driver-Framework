#include "../drivers/gpio/gpio.h"

int main(void)
{
    /*
     * Initialize GPIO driver.
     */
    GPIO_Init();

    /*
     * Turn GPIOC Pin 13 HIGH.
     */
    GPIO_Write(GPIO_STATE_HIGH);

    /*
     * Main loop.
     */
    while (1)
    {
        /*
         * GPIO driver test.
         */
    }

    return 0;
}