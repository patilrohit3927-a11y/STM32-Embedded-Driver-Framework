#include "bmp280.h"
#include "../i2c/i2c.h"


/* ============================================================
 * BMP280 CALIBRATION DATA
 * ============================================================ */

static uint16_t dig_T1;
static int16_t  dig_T2;
static int16_t  dig_T3;

static uint16_t dig_P1;
static int16_t  dig_P2;
static int16_t  dig_P3;
static int16_t  dig_P4;
static int16_t  dig_P5;
static int16_t  dig_P6;
static int16_t  dig_P7;
static int16_t  dig_P8;
static int16_t  dig_P9;


/* ============================================================
 * BMP280 FINE TEMPERATURE
 * ============================================================ */

static int32_t t_fine;


/* ============================================================
 * BMP280 REGISTER WRITE
 * ============================================================ */

static int BMP280_WriteRegister(
    uint8_t reg,
    uint8_t value)
{
    uint8_t data[2];

    data[0] = reg;
    data[1] = value;

    return I2C_Write(
        BMP280_I2C_ADDRESS,
        data,
        2U
    );
}


/* ============================================================
 * BMP280 REGISTER READ
 * ============================================================ */

static int BMP280_ReadRegisters(
    uint8_t reg,
    uint8_t *data,
    uint8_t length)
{
    /*
     * Select register.
     */
    if (!I2C_Write(
            BMP280_I2C_ADDRESS,
            &reg,
            1U))
    {
        return 0;
    }

    /*
     * Read requested bytes.
     */
    if (!I2C_Read(
            BMP280_I2C_ADDRESS,
            data,
            length))
    {
        return 0;
    }

    return 1;
}


/* ============================================================
 * BMP280 CHIP ID
 * ============================================================ */

int BMP280_ReadChipID(uint8_t *chip_id)
{
    if (chip_id == 0)
    {
        return 0;
    }

    if (!BMP280_ReadRegisters(
            BMP280_REG_CHIP_ID,
            chip_id,
            1U))
    {
        return 0;
    }

    return 1;
}


/* ============================================================
 * BMP280 CALIBRATION DATA
 * ============================================================ */

static int BMP280_ReadCalibration(void)
{
    uint8_t data[24];


    /*
     * Read calibration registers:
     *
     * 0x88 ... 0x9F
     *
     * 24 bytes total.
     */
    if (!BMP280_ReadRegisters(
            BMP280_REG_CALIB_START,
            data,
            24U))
    {
        return 0;
    }


    /*
     * Temperature calibration.
     */

    dig_T1 =
        (uint16_t)data[0] |
        ((uint16_t)data[1] << 8);

    dig_T2 =
        (int16_t)(
            (uint16_t)data[2] |
            ((uint16_t)data[3] << 8)
        );

    dig_T3 =
        (int16_t)(
            (uint16_t)data[4] |
            ((uint16_t)data[5] << 8)
        );


    /*
     * Pressure calibration.
     */

    dig_P1 =
        (uint16_t)data[6] |
        ((uint16_t)data[7] << 8);

    dig_P2 =
        (int16_t)(
            (uint16_t)data[8] |
            ((uint16_t)data[9] << 8)
        );

    dig_P3 =
        (int16_t)(
            (uint16_t)data[10] |
            ((uint16_t)data[11] << 8)
        );

    dig_P4 =
        (int16_t)(
            (uint16_t)data[12] |
            ((uint16_t)data[13] << 8)
        );

    dig_P5 =
        (int16_t)(
            (uint16_t)data[14] |
            ((uint16_t)data[15] << 8)
        );

    dig_P6 =
        (int16_t)(
            (uint16_t)data[16] |
            ((uint16_t)data[17] << 8)
        );

    dig_P7 =
        (int16_t)(
            (uint16_t)data[18] |
            ((uint16_t)data[19] << 8)
        );

    dig_P8 =
        (int16_t)(
            (uint16_t)data[20] |
            ((uint16_t)data[21] << 8)
        );

    dig_P9 =
        (int16_t)(
            (uint16_t)data[22] |
            ((uint16_t)data[23] << 8)
        );


    /*
     * dig_P1 must not be zero.
     */
    if (dig_P1 == 0U)
    {
        return 0;
    }

    return 1;
}


/* ============================================================
 * BMP280 INITIALIZATION
 * ============================================================ */

int BMP280_Init(void)
{
    uint8_t chip_id;


    /*
     * Read chip ID.
     */
    if (!BMP280_ReadChipID(&chip_id))
    {
        return BMP280_STATUS_ERROR;
    }


    /*
     * Verify BMP280.
     */
    if (chip_id != BMP280_CHIP_ID)
    {
        return BMP280_STATUS_INVALID_ID;
    }


    /*
     * Read factory calibration values.
     */
    if (!BMP280_ReadCalibration())
    {
        return BMP280_STATUS_ERROR;
    }


    /*
     * CONFIG register:
     *
     * Standby = 0.5 ms
     * Filter  = OFF
     * SPI     = OFF
     */
    if (!BMP280_WriteRegister(
            BMP280_REG_CONFIG,
            0x00U))
    {
        return BMP280_STATUS_ERROR;
    }


    /*
     * CTRL_MEAS:
     *
     * Temperature oversampling = x1
     * Pressure oversampling    = x1
     * Normal mode
     *
     * 0x27 = 0010 0111
     */
    if (!BMP280_WriteRegister(
            BMP280_REG_CTRL_MEAS,
            0x27U))
    {
        return BMP280_STATUS_ERROR;
    }


    return BMP280_STATUS_OK;
}


/* ============================================================
 * BMP280 READ RAW DATA
 * ============================================================ */

static int BMP280_ReadRaw(
    uint32_t *adc_pressure,
    uint32_t *adc_temperature)
{
    uint8_t data[6];


    if (adc_pressure == 0 ||
        adc_temperature == 0)
    {
        return 0;
    }


    /*
     * Read:
     *
     * F7 = pressure MSB
     * F8 = pressure LSB
     * F9 = pressure XLSB
     * FA = temperature MSB
     * FB = temperature LSB
     * FC = temperature XLSB
     */
    if (!BMP280_ReadRegisters(
            BMP280_REG_PRESSURE_MSB,
            data,
            6U))
    {
        return 0;
    }


    /*
     * Pressure is 20-bit.
     */
    *adc_pressure =
        ((uint32_t)data[0] << 12) |
        ((uint32_t)data[1] << 4) |
        ((uint32_t)data[2] >> 4);


    /*
     * Temperature is 20-bit.
     */
    *adc_temperature =
        ((uint32_t)data[3] << 12) |
        ((uint32_t)data[4] << 4) |
        ((uint32_t)data[5] >> 4);


    return 1;
}


/* ============================================================
 * BMP280 READ AND COMPENSATE
 * ============================================================ */

int BMP280_ReadData(
    int32_t *temperature_centi,
    uint32_t *pressure_pa)
{
    uint32_t adc_temperature;
    uint32_t adc_pressure;

    int32_t var1;
    int32_t var2;

    int64_t p_var1;
    int64_t p_var2;
    int64_t p;


    if (temperature_centi == 0 ||
        pressure_pa == 0)
    {
        return 0;
    }


    /*
     * Read raw pressure and temperature.
     */
    if (!BMP280_ReadRaw(
            &adc_pressure,
            &adc_temperature))
    {
        return 0;
    }


    /* ========================================================
     * TEMPERATURE COMPENSATION
     * ======================================================== */

    var1 =
        ((((int32_t)(adc_temperature >> 3) -
           ((int32_t)dig_T1 << 1))) *
          (int32_t)dig_T2) >> 11;


    var2 =
        (((((int32_t)(adc_temperature >> 4) -
            (int32_t)dig_T1) *
           ((int32_t)(adc_temperature >> 4) -
            (int32_t)dig_T1)) >> 12) *
         (int32_t)dig_T3) >> 14;


    t_fine = var1 + var2;


    /*
     * Temperature in 0.01 degree Celsius.
     */
    *temperature_centi =
        (t_fine * 5 + 128) >> 8;


    /* ========================================================
     * PRESSURE COMPENSATION
     *
     * Bosch 64-bit integer compensation formula.
     * ======================================================== */

    p_var1 = (int64_t)t_fine - 128000;

    p_var2 = p_var1 * p_var1 * (int64_t)dig_P6;

    p_var2 =
        p_var2 +
        ((p_var1 * (int64_t)dig_P5) << 17);

    p_var2 =
        p_var2 +
        ((int64_t)dig_P4 << 35);


    p_var1 =
        ((p_var1 * p_var1 * (int64_t)dig_P3) >> 8) +
        ((p_var1 * (int64_t)dig_P2) << 12);


    p_var1 =
        (((int64_t)1 << 47) + p_var1) *
        (int64_t)dig_P1;

    p_var1 >>= 33;


    if (p_var1 == 0)
    {
        return 0;
    }


    p =
        1048576 -
        (int64_t)adc_pressure;


    p =
        (((p << 31) - p_var2) * 3125) /
        p_var1;


    p_var1 =
        ((int64_t)dig_P9 *
         (p >> 13) *
         (p >> 13)) >> 25;


    p_var2 =
        ((int64_t)dig_P8 * p) >> 19;


    p =
        ((p + p_var1 + p_var2) >> 8) +
        ((int64_t)dig_P7 << 4);


    /*
     * Convert from Q24.8 Pa to Pa.
     */
    *pressure_pa = (uint32_t)(p >> 8);


    return 1;
}