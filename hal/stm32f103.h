#ifndef STM32F103_H
#define STM32F103_H

#include <stdint.h>


/*
 * ============================================================
 * STM32F103C8T6 HARDWARE REGISTER DEFINITIONS
 * ============================================================
 *
 * MCU:
 * STM32F103C8T6
 *
 * Core:
 * ARM Cortex-M3
 *
 * Project clock assumption:
 * HSI = 8 MHz
 */


/* ============================================================
 * GPIO REGISTER STRUCTURE
 * ============================================================ */

typedef struct
{
    volatile uint32_t CRL;       /* 0x00 */
    volatile uint32_t CRH;       /* 0x04 */
    volatile uint32_t IDR;       /* 0x08 */
    volatile uint32_t ODR;       /* 0x0C */
    volatile uint32_t BSRR;      /* 0x10 */
    volatile uint32_t BRR;       /* 0x14 */
    volatile uint32_t LCKR;      /* 0x18 */

} GPIO_TypeDef;


/* GPIO base addresses */

#define GPIOA_BASE          0x40010800UL
#define GPIOB_BASE          0x40010C00UL
#define GPIOC_BASE          0x40011000UL


/* GPIO peripheral pointers */

#define GPIOA               ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *)GPIOC_BASE)


/* GPIO pin definitions */

#define GPIO_PIN_0          (1UL << 0)
#define GPIO_PIN_1          (1UL << 1)
#define GPIO_PIN_2          (1UL << 2)
#define GPIO_PIN_3          (1UL << 3)
#define GPIO_PIN_4          (1UL << 4)
#define GPIO_PIN_5          (1UL << 5)
#define GPIO_PIN_6          (1UL << 6)
#define GPIO_PIN_7          (1UL << 7)
#define GPIO_PIN_8          (1UL << 8)
#define GPIO_PIN_9          (1UL << 9)
#define GPIO_PIN_10         (1UL << 10)
#define GPIO_PIN_11         (1UL << 11)
#define GPIO_PIN_12         (1UL << 12)
#define GPIO_PIN_13         (1UL << 13)
#define GPIO_PIN_14         (1UL << 14)
#define GPIO_PIN_15         (1UL << 15)


/* ============================================================
 * RCC REGISTER STRUCTURE
 * ============================================================ */

typedef struct
{
    volatile uint32_t CR;        /* 0x00 */
    volatile uint32_t CFGR;      /* 0x04 */
    volatile uint32_t CIR;       /* 0x08 */
    volatile uint32_t APB2RSTR;  /* 0x0C */
    volatile uint32_t APB1RSTR;  /* 0x10 */
    volatile uint32_t AHBENR;    /* 0x14 */
    volatile uint32_t APB2ENR;   /* 0x18 */
    volatile uint32_t APB1ENR;   /* 0x1C */
    volatile uint32_t BDCR;      /* 0x20 */
    volatile uint32_t CSR;       /* 0x24 */

} RCC_TypeDef;


#define RCC_BASE            0x40021000UL

#define RCC                 ((RCC_TypeDef *)RCC_BASE)


/* ============================================================
 * RCC APB2 CLOCK ENABLE BITS
 * ============================================================ */

/* AFIO clock */
#define RCC_APB2ENR_AFIOEN       (1UL << 0)

/* GPIOA clock */
#define RCC_APB2ENR_IOPAEN       (1UL << 2)

/* GPIOB clock */
#define RCC_APB2ENR_IOPBEN       (1UL << 3)

/* GPIOC clock */
#define RCC_APB2ENR_IOPCEN       (1UL << 4)

/* SPI1 clock */
#define RCC_APB2ENR_SPI1EN       (1UL << 12)

/* USART1 clock */
#define RCC_APB2ENR_USART1EN     (1UL << 14)


/* ============================================================
 * RCC APB1 CLOCK ENABLE BITS
 * ============================================================ */

/* I2C1 clock */
#define RCC_APB1ENR_I2C1EN       (1UL << 21)


/* ============================================================
 * USART REGISTER STRUCTURE
 * ============================================================ */

typedef struct
{
    volatile uint32_t SR;        /* 0x00 */
    volatile uint32_t DR;        /* 0x04 */
    volatile uint32_t BRR;       /* 0x08 */
    volatile uint32_t CR1;       /* 0x0C */
    volatile uint32_t CR2;       /* 0x10 */
    volatile uint32_t CR3;       /* 0x14 */
    volatile uint32_t GTPR;      /* 0x18 */

} USART_TypeDef;


#define USART1_BASE         0x40013800UL

#define USART1              ((USART_TypeDef *)USART1_BASE)


/* ============================================================
 * USART STATUS REGISTER BITS
 * ============================================================ */

#define USART_SR_RXNE       (1UL << 5)
#define USART_SR_TXE        (1UL << 7)


/* ============================================================
 * USART CONTROL REGISTER 1 BITS
 * ============================================================ */

#define USART_CR1_RE        (1UL << 2)
#define USART_CR1_TE        (1UL << 3)
#define USART_CR1_RXNEIE    (1UL << 5)
#define USART_CR1_UE        (1UL << 13)


/* ============================================================
 * SPI REGISTER STRUCTURE
 * ============================================================ */

typedef struct
{
    volatile uint32_t CR1;       /* 0x00 */
    volatile uint32_t CR2;       /* 0x04 */
    volatile uint32_t SR;        /* 0x08 */
    volatile uint32_t DR;        /* 0x0C */
    volatile uint32_t CRCPR;     /* 0x10 */
    volatile uint32_t RXCRCR;    /* 0x14 */
    volatile uint32_t TXCRCR;    /* 0x18 */
    volatile uint32_t I2SCFGR;   /* 0x1C */
    volatile uint32_t I2SPR;     /* 0x20 */

} SPI_TypeDef;


#define SPI1_BASE           0x40013000UL

#define SPI1                ((SPI_TypeDef *)SPI1_BASE)


/* ============================================================
 * SPI CONTROL REGISTER 1 BITS
 * ============================================================ */

#define SPI_CR1_CPHA        (1UL << 0)
#define SPI_CR1_CPOL        (1UL << 1)
#define SPI_CR1_MSTR        (1UL << 2)

#define SPI_CR1_BR0         (1UL << 3)
#define SPI_CR1_BR1         (1UL << 4)
#define SPI_CR1_BR2         (1UL << 5)

#define SPI_CR1_SPE         (1UL << 6)
#define SPI_CR1_LSBFIRST    (1UL << 7)

#define SPI_CR1_SSI         (1UL << 8)
#define SPI_CR1_SSM         (1UL << 9)


/* ============================================================
 * SPI STATUS REGISTER BITS
 * ============================================================ */

#define SPI_SR_RXNE         (1UL << 0)
#define SPI_SR_TXE          (1UL << 1)
#define SPI_SR_BSY          (1UL << 7)


/* ============================================================
 * I2C REGISTER STRUCTURE
 * ============================================================ */

typedef struct
{
    volatile uint32_t CR1;       /* 0x00 */
    volatile uint32_t CR2;       /* 0x04 */
    volatile uint32_t OAR1;      /* 0x08 */
    volatile uint32_t OAR2;      /* 0x0C */
    volatile uint32_t DR;        /* 0x10 */
    volatile uint32_t SR1;       /* 0x14 */
    volatile uint32_t SR2;       /* 0x18 */
    volatile uint32_t CCR;       /* 0x1C */
    volatile uint32_t TRISE;     /* 0x20 */
    volatile uint32_t FLTR;      /* 0x24 */

} I2C_TypeDef;


/* I2C1 base address */

#define I2C1_BASE           0x40005400UL

#define I2C1                ((I2C_TypeDef *)I2C1_BASE)


/* ============================================================
 * I2C CONTROL REGISTER 1 BITS
 * ============================================================ */

#define I2C_CR1_PE          (1UL << 0)
#define I2C_CR1_START       (1UL << 8)
#define I2C_CR1_STOP        (1UL << 9)
#define I2C_CR1_ACK         (1UL << 10)

/* Software reset */
#define I2C_CR1_SWRST      (1UL << 15)


/* ============================================================
 * I2C STATUS REGISTER 1 BITS
 * ============================================================ */

#define I2C_SR1_SB          (1UL << 0)
#define I2C_SR1_ADDR        (1UL << 1)
#define I2C_SR1_BTF         (1UL << 2)
#define I2C_SR1_TXE         (1UL << 7)

#define I2C_SR1_BERR        (1UL << 8)
#define I2C_SR1_ARLO        (1UL << 9)
#define I2C_SR1_AF          (1UL << 10)
#define I2C_SR1_OVR         (1UL << 11)


/* ============================================================
 * I2C STATUS REGISTER 2 BITS
 * ============================================================ */

#define I2C_SR2_BUSY        (1UL << 1)


/* ============================================================
 * NVIC
 * ============================================================ */

#define NVIC_ISER_BASE      0xE000E100UL

#define NVIC_ICER_BASE      0xE000E180UL


/* ============================================================
 * NVIC ENABLE IRQ
 * ============================================================ */

static inline void NVIC_EnableIRQ(uint32_t irq)
{
    volatile uint32_t *iser;

    if (irq < 32U)
    {
        iser = (volatile uint32_t *)0xE000E100UL;

        *iser = (1UL << irq);
    }
    else
    {
        iser = (volatile uint32_t *)0xE000E104UL;

        *iser = (1UL << (irq - 32U));
    }
}


/* ============================================================
 * INTERRUPT NUMBERS
 * ============================================================ */

#define USART1_IRQn         37U


#endif /* STM32F103_H */