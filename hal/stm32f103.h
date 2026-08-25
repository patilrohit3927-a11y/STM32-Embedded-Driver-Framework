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
 * Flash:
 * 64 KB
 *
 * SRAM:
 * 20 KB
 */


/* ============================================================
 * BASE ADDRESSES
 * ============================================================ */

#define PERIPH_BASE             0x40000000UL

#define APB1PERIPH_BASE         0x40000000UL
#define APB2PERIPH_BASE         0x40010000UL
#define AHBPERIPH_BASE          0x40018000UL

/*
 * STM32F103 RCC
 *
 * RCC base address = 0x40021000
 */
#define RCC_BASE                0x40021000UL

#define GPIOA_BASE              0x40010800UL
#define GPIOB_BASE              0x40010C00UL
#define GPIOC_BASE              0x40011000UL

#define SPI1_BASE               0x40013000UL

#define USART1_BASE             0x40013800UL


/* ============================================================
 * NVIC
 * ============================================================ */

#define NVIC_ISER0              (*(volatile uint32_t *)0xE000E100UL)
#define NVIC_ISER1              (*(volatile uint32_t *)0xE000E104UL)

/*
 * USART1 interrupt number.
 *
 * STM32F103:
 *
 * USART1 IRQ = 37
 */
#define USART1_IRQn             37U


static inline void NVIC_EnableIRQ(uint32_t IRQn)
{
    if (IRQn < 32U)
    {
        NVIC_ISER0 = (uint32_t)(1UL << IRQn);
    }
    else
    {
        NVIC_ISER1 = (uint32_t)(1UL << (IRQn - 32U));
    }
}


/* ============================================================
 * RCC
 * ============================================================ */

typedef struct
{
    volatile uint32_t CR;          /* 0x00 */
    volatile uint32_t CFGR;        /* 0x04 */
    volatile uint32_t CIR;         /* 0x08 */
    volatile uint32_t APB2RSTR;    /* 0x0C */
    volatile uint32_t APB1RSTR;    /* 0x10 */
    volatile uint32_t AHBENR;      /* 0x14 */
    volatile uint32_t APB2ENR;     /* 0x18 */
    volatile uint32_t APB1ENR;     /* 0x1C */
    volatile uint32_t BDCR;        /* 0x20 */
    volatile uint32_t CSR;         /* 0x24 */

} RCC_TypeDef;


/* ============================================================
 * GPIO
 * ============================================================ */

typedef struct
{
    volatile uint32_t CRL;         /* 0x00 */
    volatile uint32_t CRH;         /* 0x04 */
    volatile uint32_t IDR;         /* 0x08 */
    volatile uint32_t ODR;         /* 0x0C */
    volatile uint32_t BSRR;        /* 0x10 */
    volatile uint32_t BRR;         /* 0x14 */
    volatile uint32_t LCKR;        /* 0x18 */

} GPIO_TypeDef;


/* ============================================================
 * USART
 * ============================================================ */

typedef struct
{
    volatile uint32_t SR;          /* 0x00 */
    volatile uint32_t DR;          /* 0x04 */
    volatile uint32_t BRR;         /* 0x08 */
    volatile uint32_t CR1;         /* 0x0C */
    volatile uint32_t CR2;         /* 0x10 */
    volatile uint32_t CR3;         /* 0x14 */
    volatile uint32_t GTPR;        /* 0x18 */

} USART_TypeDef;


/* ============================================================
 * SPI
 * ============================================================ */

typedef struct
{
    volatile uint32_t CR1;         /* 0x00 */
    volatile uint32_t CR2;         /* 0x04 */
    volatile uint32_t SR;          /* 0x08 */
    volatile uint32_t DR;          /* 0x0C */
    volatile uint32_t CRCPR;       /* 0x10 */
    volatile uint32_t RXCRCR;      /* 0x14 */
    volatile uint32_t TXCRCR;      /* 0x18 */
    volatile uint32_t I2SCFGR;     /* 0x1C */
    volatile uint32_t I2SPR;       /* 0x20 */

} SPI_TypeDef;


/* ============================================================
 * PERIPHERAL POINTERS
 * ============================================================ */

#define RCC                     ((RCC_TypeDef *)RCC_BASE)

#define GPIOA                   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC                   ((GPIO_TypeDef *)GPIOC_BASE)

#define USART1                  ((USART_TypeDef *)USART1_BASE)

#define SPI1                    ((SPI_TypeDef *)SPI1_BASE)


/* ============================================================
 * RCC APB2 ENABLE BITS
 * ============================================================ */

#define RCC_APB2ENR_IOPAEN      (1UL << 2)
#define RCC_APB2ENR_IOPBEN      (1UL << 3)
#define RCC_APB2ENR_IOPCEN      (1UL << 4)

#define RCC_APB2ENR_SPI1EN      (1UL << 12)

#define RCC_APB2ENR_USART1EN    (1UL << 14)


/* ============================================================
 * GPIO PINS
 * ============================================================ */

#define GPIO_PIN_0              (1UL << 0)
#define GPIO_PIN_1              (1UL << 1)
#define GPIO_PIN_2              (1UL << 2)
#define GPIO_PIN_3              (1UL << 3)
#define GPIO_PIN_4              (1UL << 4)
#define GPIO_PIN_5              (1UL << 5)
#define GPIO_PIN_6              (1UL << 6)
#define GPIO_PIN_7              (1UL << 7)
#define GPIO_PIN_8              (1UL << 8)
#define GPIO_PIN_9              (1UL << 9)
#define GPIO_PIN_10             (1UL << 10)
#define GPIO_PIN_11             (1UL << 11)
#define GPIO_PIN_12             (1UL << 12)
#define GPIO_PIN_13             (1UL << 13)
#define GPIO_PIN_14             (1UL << 14)
#define GPIO_PIN_15             (1UL << 15)


/* ============================================================
 * USART STATUS REGISTER
 * ============================================================ */

#define USART_SR_TXE            (1UL << 7)
#define USART_SR_TC             (1UL << 6)
#define USART_SR_RXNE           (1UL << 5)
#define USART_SR_ORE            (1UL << 3)
#define USART_SR_NE             (1UL << 2)
#define USART_SR_FE             (1UL << 1)
#define USART_SR_PE             (1UL << 0)


/* ============================================================
 * USART CONTROL REGISTER 1
 * ============================================================ */

#define USART_CR1_UE            (1UL << 13)
#define USART_CR1_TE            (1UL << 3)
#define USART_CR1_RE            (1UL << 2)
#define USART_CR1_RXNEIE        (1UL << 5)


/* ============================================================
 * SPI CR1
 * ============================================================ */

#define SPI_CR1_CPHA            (1UL << 0)
#define SPI_CR1_CPOL            (1UL << 1)
#define SPI_CR1_MSTR            (1UL << 2)

#define SPI_CR1_BR0             (1UL << 3)
#define SPI_CR1_BR1             (1UL << 4)
#define SPI_CR1_BR2             (1UL << 5)

#define SPI_CR1_SPE             (1UL << 6)
#define SPI_CR1_LSBFIRST        (1UL << 7)
#define SPI_CR1_SSI             (1UL << 8)
#define SPI_CR1_SSM             (1UL << 9)


/* ============================================================
 * SPI STATUS REGISTER
 * ============================================================ */

#define SPI_SR_RXNE             (1UL << 0)
#define SPI_SR_TXE              (1UL << 1)
#define SPI_SR_BSY              (1UL << 7)


#endif /* STM32F103_H */