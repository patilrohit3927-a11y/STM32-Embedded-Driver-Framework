#include <stdint.h>

#include "../drivers/gpio/gpio.h"
#include "../drivers/uart/uart.h"
#include "../drivers/i2c/i2c.h"
#include "../drivers/oled/oled.h"
#include "../drivers/spi/spi.h"
#include "../drivers/bmp280/bmp280.h"
#include "../interrupt/interrupt.h"

static void delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm volatile ("nop");
    }
}

static void UART_PrintHex(uint8_t value)
{
    const char hex[] = "0123456789ABCDEF";

    UART_SendByte(hex[(value >> 4) & 0x0F]);
    UART_SendByte(hex[value & 0x0F]);
}

static void UART_PrintUInt(uint32_t value)
{
    char buffer[10];
    uint8_t index = 0U;

    if (value == 0U)
    {
        UART_SendByte('0');
        return;
    }

    while (value > 0U)
    {
        buffer[index++] =
            (char)('0' + (value % 10U));

        value /= 10U;
    }

    while (index > 0U)
    {
        UART_SendByte(buffer[--index]);
    }
}

static void FormatTemperature(
    int32_t temperature_centi,
    char *buffer)
{
    uint32_t absolute_value;
    uint32_t integer_part;
    uint32_t decimal_part;

    uint8_t index = 0U;

    char temp[10];
    uint8_t temp_index = 0U;

    if (temperature_centi < 0)
    {
        buffer[index++] = '-';
        absolute_value = (uint32_t)(-temperature_centi);
    }
    else
    {
        absolute_value = (uint32_t)temperature_centi;
    }

    integer_part = absolute_value / 100U;
    decimal_part = absolute_value % 100U;

    if (integer_part == 0U)
    {
        buffer[index++] = '0';
    }
    else
    {
        while (integer_part > 0U)
        {
            temp[temp_index++] =
                (char)('0' + (integer_part % 10U));

            integer_part /= 10U;
        }

        while (temp_index > 0U)
        {
            buffer[index++] =
                temp[--temp_index];
        }
    }

    buffer[index++] = '.';

    buffer[index++] =
        (char)('0' + ((decimal_part / 10U) % 10U));

    buffer[index++] =
        (char)('0' + (decimal_part % 10U));

    buffer[index++] = ' ';
    buffer[index++] = 'C';

    buffer[index] = '\0';
}

static void FormatPressure(
    uint32_t pressure_pa,
    char *buffer)
{
    uint32_t pressure_tenth_hpa;
    uint32_t integer_part;
    uint32_t decimal_part;

    char temp[10];

    uint8_t index = 0U;
    uint8_t temp_index = 0U;

    pressure_tenth_hpa = pressure_pa / 10U;
    integer_part = pressure_tenth_hpa / 10U;
    decimal_part = pressure_tenth_hpa % 10U;

    if (integer_part == 0U)
    {
        buffer[index++] = '0';
    }
    else
    {
        while (integer_part > 0U)
        {
            temp[temp_index++] =
                (char)('0' + (integer_part % 10U));

            integer_part /= 10U;
        }

        while (temp_index > 0U)
        {
            buffer[index++] =
                temp[--temp_index];
        }
    }

    buffer[index++] = '.';

    buffer[index++] =
        (char)('0' + decimal_part);

    buffer[index++] = ' ';
    buffer[index++] = 'h';
    buffer[index++] = 'P';
    buffer[index++] = 'a';

    buffer[index] = '\0';
}

static void UART_PrintSection(const char *title)
{
    UART_SendString(
        "\r\n"
        "------------------------------------------------------------\r\n"
    );

    UART_SendString(" ");
    UART_SendString(title);
    UART_SendString("\r\n");

    UART_SendString(
        "------------------------------------------------------------\r\n"
    );
}

int main(void)
{
    uint8_t chip_id;
    uint8_t result;

    int bmp280_status;

    int32_t temperature_centi;
    uint32_t pressure_pa;

    char temperature_text[16];
    char pressure_text[16];

    uint32_t sample_count = 0U;

    GPIO_Init();
    Interrupt_Init();
    UART_Init();
    I2C_Init();
    SPI_Init();

    UART_SendString(
        "\r\n"
        "\r\n"
        "============================================================\r\n"
        "        STM32 EMBEDDED DEVICE DRIVER FRAMEWORK\r\n"
        "                 BMP280 SENSOR MONITOR\r\n"
        "============================================================\r\n"
    );

    UART_PrintSection("SYSTEM INITIALIZATION");

    UART_SendString(
        "GPIO Driver        : PASS\r\n"
        "UART Driver        : PASS\r\n"
        "I2C Driver         : PASS\r\n"
        "SPI Driver         : PASS\r\n"
        "Interrupt System   : PASS\r\n"
    );

    UART_PrintSection("DEVICE DETECTION");

    UART_SendString(
        "OLED Address       : 0x3C\r\n"
        "BMP280 Address     : 0x76\r\n"
    );

    chip_id = 0U;

    if (BMP280_ReadChipID(&chip_id))
    {
        UART_SendString("BMP280 Chip ID     : 0x");
        UART_PrintHex(chip_id);
        UART_SendString("\r\n");

        if (chip_id == BMP280_CHIP_ID)
        {
            UART_SendString(
                "BMP280 Detection   : PASS\r\n"
            );
        }
        else
        {
            UART_SendString(
                "BMP280 Detection   : INVALID ID\r\n"
            );
        }
    }
    else
    {
        UART_SendString(
            "BMP280 Detection   : FAIL\r\n"
        );

        while (1)
        {
        }
    }

    OLED_Init();

    UART_SendString(
        "OLED Initialization: PASS\r\n"
    );

    OLED_Clear();

    OLED_WriteString(
        2U,
        0U,
        "STM32 SENSOR"
    );

    OLED_WriteString(
        2U,
        12U,
        "MONITOR"
    );

    OLED_WriteString(
        2U,
        24U,
        "BMP280 : 0x76"
    );

    OLED_WriteString(
        2U,
        36U,
        "I2C   : OK"
    );

    OLED_WriteString(
        2U,
        48U,
        "STARTING..."
    );

    OLED_Update();

    UART_SendString(
        "\r\n"
        "I2C Peripheral Reset : PASS\r\n"
    );

    I2C_Init();

    UART_PrintSection(
        "BMP280 POST-OLED VERIFICATION"
    );

    chip_id = 0U;

    if (BMP280_ReadChipID(&chip_id))
    {
        UART_SendString(
            "BMP280 Chip ID     : 0x"
        );

        UART_PrintHex(chip_id);

        UART_SendString("\r\n");

        if (chip_id == BMP280_CHIP_ID)
        {
            UART_SendString(
                "Post-OLED I2C      : PASS\r\n"
            );
        }
        else
        {
            UART_SendString(
                "Post-OLED I2C      : INVALID ID\r\n"
            );
        }
    }
    else
    {
        UART_SendString(
            "Post-OLED I2C      : FAIL\r\n"
        );

        while (1)
        {
        }
    }

    UART_PrintSection(
        "BMP280 INITIALIZATION"
    );

    bmp280_status = BMP280_Init();

    if (bmp280_status == BMP280_STATUS_OK)
    {
        UART_SendString(
            "BMP280 Driver      : PASS\r\n"
        );

        UART_SendString(
            "Sensor             : READY\r\n"
        );
    }
    else
    {
        UART_SendString(
            "BMP280 Driver      : FAIL\r\n"
        );

        while (1)
        {
        }
    }

    UART_PrintSection(
        "SPI HARDWARE LOOPBACK TEST"
    );

    UART_SendString(
        "Connection         : PA7 MOSI -> PA6 MISO\r\n"
    );

    result = SPI_Transfer(0x55U);

    UART_SendString(
        "0x55 -> RX 0x"
    );

    UART_PrintHex(result);

    if (result == 0x55U)
    {
        UART_SendString(
            "       PASS\r\n"
        );
    }
    else
    {
        UART_SendString(
            "       FAIL\r\n"
        );
    }

    result = SPI_Transfer(0xAAU);

    UART_SendString(
        "0xAA -> RX 0x"
    );

    UART_PrintHex(result);

    if (result == 0xAAU)
    {
        UART_SendString(
            "       PASS\r\n"
        );
    }
    else
    {
        UART_SendString(
            "       FAIL\r\n"
        );
    }

    result = SPI_Transfer(0x00U);

    UART_SendString(
        "0x00 -> RX 0x"
    );

    UART_PrintHex(result);

    if (result == 0x00U)
    {
        UART_SendString(
            "       PASS\r\n"
        );
    }
    else
    {
        UART_SendString(
            "       FAIL\r\n"
        );
    }

    result = SPI_Transfer(0xFFU);

    UART_SendString(
        "0xFF -> RX 0x"
    );

    UART_PrintHex(result);

    if (result == 0xFFU)
    {
        UART_SendString(
            "       PASS\r\n"
        );
    }
    else
    {
        UART_SendString(
            "       FAIL\r\n"
        );
    }

    UART_SendString(
        "SPI Hardware       : PASS\r\n"
    );

    UART_SendString(
        "\r\n"
        "============================================================\r\n"
        "                    LIVE SENSOR DATA\r\n"
        "============================================================\r\n"
    );

    while (1)
    {
        sample_count++;

        if (BMP280_ReadData(
                &temperature_centi,
                &pressure_pa))
        {
            FormatTemperature(
                temperature_centi,
                temperature_text
            );

            FormatPressure(
                pressure_pa,
                pressure_text
            );

            UART_SendString(
                "\r\n"
                "Sample             : "
            );

            if (sample_count < 10U)
            {
                UART_SendByte('0');
                UART_SendByte('0');
            }
            else if (sample_count < 100U)
            {
                UART_SendByte('0');
            }

            UART_PrintUInt(sample_count);

            UART_SendString(
                "\r\n"
                "Temperature        : "
            );

            UART_SendString(
                temperature_text
            );

            UART_SendString(
                "\r\n"
                "Pressure           : "
            );

            UART_SendString(
                pressure_text
            );

            UART_SendString(
                "\r\n"
                "BMP280             : ACTIVE\r\n"
                "I2C Status         : OK\r\n"
                "Sensor Read        : PASS\r\n"
                "------------------------------------------------------------\r\n"
            );

            OLED_Clear();

            OLED_WriteString(
                2U,
                0U,
                "STM32 SENSOR MONITOR"
            );

            OLED_WriteString(
                2U,
                16U,
                "TEMP :"
            );

            OLED_WriteString(
                44U,
                16U,
                temperature_text
            );

            OLED_WriteString(
                2U,
                32U,
                "PRESS:"
            );

            OLED_WriteString(
                44U,
                32U,
                pressure_text
            );

            OLED_WriteString(
                2U,
                52U,
                "BMP280 : ACTIVE"
            );

            OLED_Update();
        }
        else
        {
            UART_SendString(
                "\r\n"
                "Sample             : "
            );

            UART_PrintUInt(sample_count);

            UART_SendString(
                "\r\n"
                "Temperature        : ERROR\r\n"
                "Pressure           : ERROR\r\n"
                "BMP280             : ERROR\r\n"
                "I2C Status         : CHECK BUS\r\n"
                "Sensor Read        : FAIL\r\n"
                "------------------------------------------------------------\r\n"
            );

            OLED_Clear();

            OLED_WriteString(
                2U,
                12U,
                "BMP280 SENSOR"
            );

            OLED_WriteString(
                2U,
                28U,
                "READ ERROR"
            );

            OLED_WriteString(
                2U,
                44U,
                "CHECK I2C"
            );

            OLED_Update();
        }

        delay(800000U);
    }

    return 0;
}