#include "spi.h"
#include "../../hal/stm32f103.h"


/*
 * ============================================================
 * SPI1 DRIVER - STM32F103C8T6
 * ============================================================
 *
 * SPI1 pins:
 *
 * PA5 -> SCK
 * PA6 -> MISO
 * PA7 -> MOSI
 *
 * NSS is handled using software NSS.
 *
 * No physical CS pin is controlled by this driver.
 */


/*
 * Timeout prevents the CPU from getting stuck forever
 * if the SPI peripheral does not respond.
 */
#define SPI_TIMEOUT_COUNT     100000UL


/* ============================================================
 * SPI INITIALIZATION
 * ============================================================ */

void SPI_Init(void)
{
    /*
     * Enable GPIOA clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /*
     * Enable SPI1 clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;


    /*
     * --------------------------------------------------------
     * PA5 = SPI1_SCK
     * --------------------------------------------------------
     *
     * Alternate function push-pull
     * Output speed = 50 MHz
     *
     * MODE = 11
     * CNF  = 10
     *
     * Configuration = 0xB
     */

    GPIOA->CRL &= ~(0xFUL << 20);
    GPIOA->CRL |=  (0xBUL << 20);


    /*
     * --------------------------------------------------------
     * PA6 = SPI1_MISO
     * --------------------------------------------------------
     *
     * Floating input.
     *
     * Configuration = 0x4
     */

    GPIOA->CRL &= ~(0xFUL << 24);
    GPIOA->CRL |=  (0x4UL << 24);


    /*
     * --------------------------------------------------------
     * PA7 = SPI1_MOSI
     * --------------------------------------------------------
     *
     * Alternate function push-pull
     * Output speed = 50 MHz
     *
     * Configuration = 0xB
     */

    GPIOA->CRL &= ~(0xFUL << 28);
    GPIOA->CRL |=  (0xBUL << 28);


    /*
     * Disable SPI before configuration.
     */
    SPI1->CR1 = 0U;


    /*
     * Configure SPI:
     *
     * MSTR = Master
     * SSM  = Software slave management
     * SSI  = Internal NSS high
     *
     * BR0  = Clock divider
     *
     * SPI clock:
     *
     * APB2 = 8 MHz
     *
     * fSPI = 8 MHz / 4
     *      = 2 MHz
     */

    SPI1->CR1 =
        SPI_CR1_MSTR |
        SPI_CR1_SSM  |
        SPI_CR1_SSI  |
        SPI_CR1_BR0;


    /*
     * Enable SPI.
     */
    SPI1->CR1 |= SPI_CR1_SPE;
}


/* ============================================================
 * SPI TRANSFER
 * ============================================================ */

uint8_t SPI_Transfer(uint8_t data)
{
    uint32_t timeout;
    uint8_t received;


    /*
     * Wait for TX register to become empty.
     */
    timeout = SPI_TIMEOUT_COUNT;

    while (!(SPI1->SR & SPI_SR_TXE))
    {
        if (--timeout == 0U)
        {
            return 0xFFU;
        }
    }


    /*
     * Send byte.
     */
    SPI1->DR = data;


    /*
     * Wait for received byte.
     */
    timeout = SPI_TIMEOUT_COUNT;

    while (!(SPI1->SR & SPI_SR_RXNE))
    {
        if (--timeout == 0U)
        {
            return 0xFFU;
        }
    }


    /*
     * Read received byte.
     */
    received = (uint8_t)SPI1->DR;


    /*
     * Wait until SPI is no longer busy.
     */
    timeout = SPI_TIMEOUT_COUNT;

    while (SPI1->SR & SPI_SR_BSY)
    {
        if (--timeout == 0U)
        {
            break;
        }
    }


    return received;
}


/* ============================================================
 * SPI SEND BYTE
 * ============================================================ */

void SPI_SendByte(uint8_t data)
{
    (void)SPI_Transfer(data);
}


/* ============================================================
 * SPI RECEIVE BYTE
 * ============================================================ */

uint8_t SPI_ReceiveByte(void)
{
    return SPI_Transfer(0xFFU);
}


/* ============================================================
 * RENODE SPI SIMULATION
 * ============================================================
 *
 * IMPORTANT:
 *
 * This function is NOT used on physical hardware.
 *
 * It exists only for the Renode demonstration.
 */

uint8_t SPI_SimulateTransfer(uint8_t data)
{
    if (data == 0x55U)
    {
        return 0xAAU;
    }

    if (data == 0xAAU)
    {
        return 0x55U;
    }

    return data;
}