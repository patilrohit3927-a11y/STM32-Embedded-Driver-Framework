#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>


/*
 * GPIO pin modes.
 */

typedef enum
{
    GPIO_INPUT = 0,
    GPIO_OUTPUT

} GPIO_Mode;


/*
 * GPIO pin states.
 */

typedef enum
{
    GPIO_STATE_LOW = 0,
    GPIO_STATE_HIGH

} GPIO_State;


/*
 * Initialize GPIOC Pin 13.
 */
void GPIO_Init(void);


/*
 * Write state to GPIOC Pin 13.
 */
void GPIO_Write(GPIO_State state);


/*
 * Toggle GPIOC Pin 13.
 */
void GPIO_Toggle(void);


/*
 * Read GPIOC Pin 13.
 */
GPIO_State GPIO_Read(void);


#endif /* GPIO_H */