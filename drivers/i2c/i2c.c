#include "i2c.h"
#include "../../hal/stm32f103.h"

#define I2C_TIMEOUT_COUNT    100000UL

/*
 * STM32F103:
 *
 * APB1RSTR bit 21 = I2C1 reset
 *
 * We use the bit directly because the current HAL header
 * does not provide an I2C1 reset macro.
 */
#define RCC_APB1RSTR_I2C1RST    (1UL << 21)

/*
 * STM32F103:
 *
 * I2C_SR1 bit 6 = RXNE
 */
#define I2C_SR1_RXNE_BIT        (1UL << 6)


/* ============================================================
 * I2C BUS FREE
 * ============================================================ */

static int I2C_WaitBusFree(void)
{
    uint32_t timeout = I2C_TIMEOUT_COUNT;

    while (I2C1->SR2 & I2C_SR2_BUSY)
    {
        if (--timeout == 0U)
            return 0;
    }

    return 1;
}


/* ============================================================
 * I2C START
 * ============================================================ */

static int I2C_Start(void)
{
    uint32_t timeout;

    /*
     * Wait until the peripheral reports that the bus
     * is free.
     */
    if (!I2C_WaitBusFree())
        return 0;

    /*
     * Make sure STOP from a previous transaction is cleared.
     */
    I2C1->CR1 &= ~I2C_CR1_STOP;

    /*
     * Generate START.
     */
    I2C1->CR1 |= I2C_CR1_START;

    timeout = I2C_TIMEOUT_COUNT;

    while (!(I2C1->SR1 & I2C_SR1_SB))
    {
        if (--timeout == 0U)
            return 0;
    }

    return 1;
}


/* ============================================================
 * I2C STOP
 * ============================================================ */

static void I2C_Stop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;
}


/* ============================================================
 * WAIT FOR I2C FLAG
 * ============================================================ */

static int I2C_WaitFlag(uint32_t flag)
{
    uint32_t timeout = I2C_TIMEOUT_COUNT;

    while (!(I2C1->SR1 & flag))
    {
        if (I2C1->SR1 & I2C_SR1_AF)
            return 0;

        if (--timeout == 0U)
            return 0;
    }

    return 1;
}


/* ============================================================
 * I2C INITIALIZATION
 * ============================================================ */

void I2C_Init(void)
{
    /*
     * Enable GPIOB clock.
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /*
     * Enable I2C1 clock.
     */
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    /*
     * --------------------------------------------------------
     * HARD RESET I2C1 PERIPHERAL
     * --------------------------------------------------------
     *
     * This is different from simply writing CR1 = 0.
     *
     * It resets the complete I2C1 peripheral state machine.
     */
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;

    /*
     * Small delay while reset is asserted.
     */
    {
        volatile uint32_t delay_count;

        for (delay_count = 0U;
             delay_count < 100U;
             delay_count++)
        {
            __asm volatile ("nop");
        }
    }

    /*
     * Release I2C1 from reset.
     */
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

    /*
     * --------------------------------------------------------
     * I2C GPIO CONFIGURATION
     * --------------------------------------------------------
     *
     * PB6 = I2C1_SCL
     * PB7 = I2C1_SDA
     *
     * STM32F1:
     *
     * MODE = 11 -> Output 50 MHz
     * CNF  = 11 -> Alternate-function open-drain
     *
     * Configuration nibble = 0xF.
     */

    GPIOB->CRL &= ~(0xFUL << 24);
    GPIOB->CRL |=  (0xFUL << 24);

    GPIOB->CRL &= ~(0xFUL << 28);
    GPIOB->CRL |=  (0xFUL << 28);

    /*
     * --------------------------------------------------------
     * I2C TIMING
     * --------------------------------------------------------
     *
     * APB1 clock = 8 MHz
     *
     * Standard-mode I2C = 100 kHz
     */

    I2C1->CR1 = 0U;

    /*
     * APB1 frequency in MHz.
     */
    I2C1->CR2 = 8U;

    /*
     * Standard-mode:
     *
     * 8 MHz / (2 * 40) = 100 kHz
     */
    I2C1->CCR = 40U;

    /*
     * Maximum SCL rise time:
     *
     * APB1 MHz + 1 = 9
     */
    I2C1->TRISE = 9U;

    /*
     * Enable ACK.
     */
    I2C1->CR1 |= I2C_CR1_ACK;

    /*
     * Enable I2C peripheral.
     */
    I2C1->CR1 |= I2C_CR1_PE;
}


/* ============================================================
 * I2C WRITE
 * ============================================================ */

int I2C_Write(
    uint8_t address,
    const uint8_t *data,
    uint8_t length)
{
    uint32_t timeout;
    uint8_t i;

    if (data == 0 || length == 0U)
        return 0;

    /*
     * Generate START.
     */
    if (!I2C_Start())
        return 0;

    /*
     * Send 7-bit address + WRITE bit.
     */
    I2C1->DR = (uint8_t)(address << 1);

    /*
     * Wait for address acknowledge.
     */
    timeout = I2C_TIMEOUT_COUNT;

    while (!(I2C1->SR1 & I2C_SR1_ADDR))
    {
        if (I2C1->SR1 & I2C_SR1_AF)
        {
            I2C1->SR1 &= ~I2C_SR1_AF;
            I2C_Stop();
            return 0;
        }

        if (--timeout == 0U)
        {
            I2C_Stop();
            return 0;
        }
    }

    /*
     * Clear ADDR.
     *
     * STM32F1 requires:
     *
     * 1. Read SR1
     * 2. Read SR2
     */
    (void)I2C1->SR1;
    (void)I2C1->SR2;

    /*
     * Send data.
     */
    for (i = 0U; i < length; i++)
    {
        if (!I2C_WaitFlag(I2C_SR1_TXE))
        {
            I2C1->SR1 &= ~I2C_SR1_AF;
            I2C_Stop();
            return 0;
        }

        I2C1->DR = data[i];
    }

    /*
     * Wait until the final byte is completely transmitted.
     */
    timeout = I2C_TIMEOUT_COUNT;

    while (!(I2C1->SR1 & I2C_SR1_BTF))
    {
        if (I2C1->SR1 & I2C_SR1_AF)
        {
            I2C1->SR1 &= ~I2C_SR1_AF;
            I2C_Stop();
            return 0;
        }

        if (--timeout == 0U)
        {
            I2C_Stop();
            return 0;
        }
    }

    /*
     * Generate STOP.
     */
    I2C_Stop();

    return 1;
}


/* ============================================================
 * I2C READ
 * ============================================================ */

int I2C_Read(
    uint8_t address,
    uint8_t *data,
    uint8_t length)
{
    uint32_t timeout;
    uint8_t i;

    if (data == 0 || length == 0U)
        return 0;

    /*
     * Generate START.
     */
    if (!I2C_Start())
        return 0;

    /*
     * Send 7-bit address + READ bit.
     */
    I2C1->DR = (uint8_t)((address << 1) | 1U);

    /*
     * Wait for address acknowledge.
     */
    timeout = I2C_TIMEOUT_COUNT;

    while (!(I2C1->SR1 & I2C_SR1_ADDR))
    {
        if (I2C1->SR1 & I2C_SR1_AF)
        {
            I2C1->SR1 &= ~I2C_SR1_AF;
            I2C_Stop();
            return 0;
        }

        if (--timeout == 0U)
        {
            I2C_Stop();
            return 0;
        }
    }


    /* ========================================================
     * SINGLE BYTE READ
     * ======================================================== */

    if (length == 1U)
    {
        /*
         * Disable ACK.
         *
         * The single received byte must be NACKed.
         */
        I2C1->CR1 &= ~I2C_CR1_ACK;

        /*
         * Clear ADDR.
         */
        (void)I2C1->SR1;
        (void)I2C1->SR2;

        /*
         * Generate STOP.
         */
        I2C_Stop();

        /*
         * Wait for received byte.
         */
        timeout = I2C_TIMEOUT_COUNT;

        while (!(I2C1->SR1 & I2C_SR1_RXNE_BIT))
        {
            if (--timeout == 0U)
            {
                I2C1->CR1 |= I2C_CR1_ACK;
                return 0;
            }
        }

        /*
         * Read received byte.
         */
        data[0] = (uint8_t)I2C1->DR;

        /*
         * Restore ACK.
         */
        I2C1->CR1 |= I2C_CR1_ACK;

        return 1;
    }


    /* ========================================================
     * MULTI-BYTE READ
     * ======================================================== */

    /*
     * ACK all bytes initially.
     */
    I2C1->CR1 |= I2C_CR1_ACK;

    /*
     * Clear ADDR.
     */
    (void)I2C1->SR1;
    (void)I2C1->SR2;

    for (i = 0U; i < length; i++)
    {
        /*
         * Before the final byte:
         *
         * Disable ACK and generate STOP.
         */
        if (i == (uint8_t)(length - 1U))
        {
            I2C1->CR1 &= ~I2C_CR1_ACK;

            I2C_Stop();
        }

        /*
         * Wait for received byte.
         */
        timeout = I2C_TIMEOUT_COUNT;

        while (!(I2C1->SR1 & I2C_SR1_RXNE_BIT))
        {
            if (--timeout == 0U)
            {
                I2C1->CR1 |= I2C_CR1_ACK;
                return 0;
            }
        }

        /*
         * Read byte.
         */
        data[i] = (uint8_t)I2C1->DR;
    }

    /*
     * Restore ACK for future transactions.
     */
    I2C1->CR1 |= I2C_CR1_ACK;

    return 1;
}