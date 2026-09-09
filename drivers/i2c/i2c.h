#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C_Init(void);

int I2C_Write(
    uint8_t address,
    const uint8_t *data,
    uint8_t length
);

int I2C_Read(
    uint8_t address,
    uint8_t *data,
    uint8_t length
);

#endif /* I2C_H */