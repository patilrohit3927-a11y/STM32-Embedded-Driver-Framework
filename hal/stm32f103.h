#ifndef STM32F103_H
#define STM32F103_H

#include <stdint.h>

/*
 * ============================================================
 * STM32F103C8T6 REGISTER DEFINITIONS
 * ============================================================
 *
 * MCU:
 * STM32F103C8T6
 * Cortex-M3
 *
 * Flash: 64 KB
 * SRAM : 20 KB
 */


/* ============================================================
 * BASE ADDRESSES
 * ============================================================ */

#define PERIPH_BASE         0x40000000UL
#define APB2PERIPH_BASE     0x40010000UL
#define AHBPERIPH_BASE      0x40020000UL


/* ============================================================
 * RCC
 * ============================================================ */

#define RCC_BASE            (AHBPERIPH_BASE + 0x00001000UL)


/* ============================================================
 * GPIO
 * ============================================================ */

#define GPIOA_BASE          (APB2PERIPH_BASE + 0x00000800UL)
#define GPIOB_BASE          (APB2PERIPH_BASE + 0x00000C00UL)
#define GPIOC_BASE          (APB2PERIPH_BASE + 0x00001000UL)


/* ============================================================
 * USART
 * ============================================================ */

#define USART1_BASE         (APB2PERIPH_BASE + 0x00003800UL)


/* ============================================================
 * RCC REGISTER STRUCTURE
 * ============================================================ */

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;

} RCC_TypeDef;


/* ============================================================
 * GPIO REGISTER STRUCTURE
 * ============================================================ */

typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;

} GPIO_TypeDef;


/* ============================================================
 * USART REGISTER STRUCTURE
 * ============================================================ */

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;

} USART_TypeDef;


/* ============================================================
 * PERIPHERAL POINTERS
 * ============================================================ */

#define RCC     ((RCC_TypeDef *)RCC_BASE)

#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)

#define USART1  ((USART_TypeDef *)USART1_BASE)


/* ============================================================
 * RCC CLOCK ENABLE BITS
 * ============================================================ */

/*
 * APB2 peripheral clock enable register.
 */

#define RCC_APB2ENR_IOPAEN      (1U << 2)
#define RCC_APB2ENR_IOPBEN      (1U << 3)
#define RCC_APB2ENR_IOPCEN      (1U << 4)

#define RCC_APB2ENR_USART1EN    (1U << 14)


/* ============================================================
 * GPIO CONFIGURATION
 * ============================================================ */

/*
 * GPIO configuration values for STM32F1.
 *
 * MODE:
 *
 * 00 = Input
 * 01 = Output 10 MHz
 * 10 = Output 2 MHz
 * 11 = Output 50 MHz
 */

#define GPIO_MODE_INPUT         0x0U
#define GPIO_MODE_OUTPUT_10MHZ  0x1U
#define GPIO_MODE_OUTPUT_2MHZ   0x2U
#define GPIO_MODE_OUTPUT_50MHZ  0x3U


/*
 * CNF values.
 */

#define GPIO_CNF_INPUT_ANALOG    0x0U
#define GPIO_CNF_INPUT_FLOATING  0x1U

#define GPIO_CNF_OUTPUT_PP       0x0U
#define GPIO_CNF_OUTPUT_OD       0x1U
#define GPIO_CNF_OUTPUT_AF_PP    0x2U
#define GPIO_CNF_OUTPUT_AF_OD    0x3U


/* ============================================================
 * GPIO PIN DEFINITIONS
 * ============================================================ */

#define GPIO_PIN_0     (1U << 0)
#define GPIO_PIN_1     (1U << 1)
#define GPIO_PIN_2     (1U << 2)
#define GPIO_PIN_3     (1U << 3)
#define GPIO_PIN_4     (1U << 4)
#define GPIO_PIN_5     (1U << 5)
#define GPIO_PIN_6     (1U << 6)
#define GPIO_PIN_7     (1U << 7)
#define GPIO_PIN_8     (1U << 8)
#define GPIO_PIN_9     (1U << 9)
#define GPIO_PIN_10    (1U << 10)
#define GPIO_PIN_11    (1U << 11)
#define GPIO_PIN_12    (1U << 12)
#define GPIO_PIN_13    (1U << 13)
#define GPIO_PIN_14    (1U << 14)
#define GPIO_PIN_15    (1U << 15)


/* ============================================================
 * GPIO OUTPUT VALUES
 * ============================================================ */

#define GPIO_LOW       0U
#define GPIO_HIGH      1U


#endif /* STM32F103_H */