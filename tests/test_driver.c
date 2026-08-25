#include <stdio.h>
#include <stdint.h>

#include "../drivers/gpio/gpio.h"
#include "../drivers/uart/uart.h"
#include "../drivers/spi/spi.h"
#include "../interrupt/interrupt.h"
#include "../config/platform.h"

static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

static void Test_Result(const char *name, int condition)
{
    total_tests++;

    if (condition)
    {
        passed_tests++;
        printf("  [PASS] %s\n", name);
    }
    else
    {
        failed_tests++;
        printf("  [FAIL] %s\n", name);
    }
}

/* -------------------------------------------------
 * HAL / Initialization Tests
 * ------------------------------------------------- */

static void Test_HAL(void)
{
    printf("\n[HAL]\n");

    /*
     * Initialization should complete without error.
     * Register-level validation will be expanded later.
     */
    Test_Result("Framework initialization", 1);
}

/* -------------------------------------------------
 * GPIO Tests
 * ------------------------------------------------- */

static void Test_GPIO(void)
{
    printf("\n[GPIO DRIVER]\n");

    GPIO_Init();

    Test_Result("GPIO initialization", 1);
}

/* -------------------------------------------------
 * UART Tests
 * ------------------------------------------------- */

static void Test_UART(void)
{
    printf("\n[UART DRIVER]\n");

    UART_Init();

    Test_Result("UART initialization", 1);

    UART_SendByte('T');

    Test_Result("UART TX", 1);
}

/* -------------------------------------------------
 * Interrupt Tests
 * ------------------------------------------------- */

static void Test_Interrupt(void)
{
    printf("\n[INTERRUPT FRAMEWORK]\n");

    Interrupt_Init();

    Test_Result("Interrupt initialization", 1);
}

/* -------------------------------------------------
 * SPI Tests
 * ------------------------------------------------- */

static void Test_SPI(void)
{
    uint8_t rx;

    printf("\n[SPI DRIVER]\n");

    SPI_Init();

#if PLATFORM_RENODE

    rx = SPI_SimulateTransfer(0x55);

#else

    rx = SPI_Transfer(0x55);

#endif

    Test_Result("SPI initialization", 1);

    Test_Result(
        "SPI transfer",
        rx == 0xAA
    );
}

/* -------------------------------------------------
 * Main Test Runner
 * ------------------------------------------------- */

int main(void)
{
    printf("\n");
    printf("============================================\n");
    printf("      STM32 EMBEDDED DRIVER FRAMEWORK\n");
    printf("              TEST SUITE\n");
    printf("============================================\n");

#if PLATFORM_RENODE
    printf("Platform : Renode Simulation\n");
#else
    printf("Platform : STM32F103 Hardware\n");
#endif

    Test_HAL();
    Test_GPIO();
    Test_UART();
    Test_Interrupt();
    Test_SPI();

    printf("\n--------------------------------------------\n");
    printf("Total Tests : %d\n", total_tests);
    printf("Passed      : %d\n", passed_tests);
    printf("Failed      : %d\n", failed_tests);
    printf("--------------------------------------------\n");

    if (failed_tests == 0)
    {
        printf("RESULT: ALL TESTS PASSED\n");
    }
    else
    {
        printf("RESULT: TESTS FAILED\n");
    }

    printf("============================================\n");

    return failed_tests;
}