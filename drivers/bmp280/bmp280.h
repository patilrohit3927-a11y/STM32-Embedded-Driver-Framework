#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>


/* ============================================================
 * BMP280 I2C ADDRESS
 * ============================================================ */

#define BMP280_I2C_ADDRESS        0x76U


/* ============================================================
 * BMP280 REGISTERS
 * ============================================================ */

#define BMP280_REG_CHIP_ID        0xD0U
#define BMP280_REG_RESET          0xE0U
#define BMP280_REG_STATUS         0xF3U
#define BMP280_REG_CTRL_MEAS      0xF4U
#define BMP280_REG_CONFIG         0xF5U

#define BMP280_REG_CALIB_START    0x88U

#define BMP280_REG_PRESSURE_MSB   0xF7U
#define BMP280_REG_PRESSURE_LSB   0xF8U
#define BMP280_REG_PRESSURE_XLSB  0xF9U

#define BMP280_REG_TEMP_MSB       0xFAU
#define BMP280_REG_TEMP_LSB       0xFBU
#define BMP280_REG_TEMP_XLSB      0xFCU


/* ============================================================
 * BMP280 CHIP ID
 * ============================================================ */

#define BMP280_CHIP_ID            0x58U


/* ============================================================
 * DRIVER STATUS
 * ============================================================ */

#define BMP280_STATUS_OK          0
#define BMP280_STATUS_ERROR       1
#define BMP280_STATUS_INVALID_ID  2


/* ============================================================
 * FUNCTION DECLARATIONS
 * ============================================================ */

int BMP280_Init(void);

int BMP280_ReadChipID(uint8_t *chip_id);

int BMP280_ReadData(
    int32_t *temperature_centi,
    uint32_t *pressure_pa
);


#endif /* BMP280_H */