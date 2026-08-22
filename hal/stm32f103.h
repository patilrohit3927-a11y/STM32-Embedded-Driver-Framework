#ifndef STM32F103_H
#define STM32F103_H

#include <stdint.h>


/* ============================================================
 * BASE ADDRESSES
 * ============================================================ */

#define APB2PERIPH_BASE     0x40010000UL
#define AHBPERIPH_BASE      0x40020000UL

#define RCC_BASE            (AHBPERIPH_BASE + 0x1000UL)

#define GPIOA_BASE          (APB2PERIPH_BASE + 0x0800UL)
#define GPIOB_BASE          (APB2PERIPH_BASE + 0x0C00UL)
#define GPIOC_BASE          (APB2PERIPH_BASE + 0x1000UL)

#define USART1_BASE         (APB2PERIPH_BASE + 0x3800UL)


/* ============================================================
 * NVIC
 * ============================================================ */

#define NVIC_ISER0          (*(volatile uint32_t *)0xE000E100UL)
#define NVIC_ISER1          (*(volatile uint32_t *)0xE000E104UL)


/*
 * USART1 interrupt number.
 */
#define USART1_IRQn         37U


/*
 * Enable interrupt.
 *
 * IRQ 0-31  -> ISER0
 * IRQ 32-63 -> ISER1
 */
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
 * GPIO
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
 * USART
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
 * RCC APB2 ENABLE BITS
 * ============================================================ */

#define RCC_APB2ENR_IOPAEN       (1U << 2)
#define RCC_APB2ENR_IOPBEN       (1U << 3)
#define RCC_APB2ENR_IOPCEN       (1U << 4)

#define RCC_APB2ENR_USART1EN     (1U << 14)


/* ============================================================
 * GPIO PINS
 * ============================================================ */

#define GPIO_PIN_0       (1U << 0)
#define GPIO_PIN_1       (1U << 1)
#define GPIO_PIN_2       (1U << 2)
#define GPIO_PIN_3       (1U << 3)
#define GPIO_PIN_4       (1U << 4)
#define GPIO_PIN_5       (1U << 5)
#define GPIO_PIN_6       (1U << 6)
#define GPIO_PIN_7       (1U << 7)
#define GPIO_PIN_8       (1U << 8)
#define GPIO_PIN_9       (1U << 9)
#define GPIO_PIN_10      (1U << 10)
#define GPIO_PIN_11      (1U << 11)
#define GPIO_PIN_12      (1U << 12)
#define GPIO_PIN_13      (1U << 13)
#define GPIO_PIN_14      (1U << 14)
#define GPIO_PIN_15      (1U << 15)


/* ============================================================
 * USART STATUS REGISTER
 * ============================================================ */

#define USART_SR_TXE      (1U << 7)
#define USART_SR_TC       (1U << 6)
#define USART_SR_RXNE     (1U << 5)


/* ============================================================
 * USART CONTROL REGISTER 1
 * ============================================================ */

#define USART_CR1_UE      (1U << 13)
#define USART_CR1_TE      (1U << 3)
#define USART_CR1_RE      (1U << 2)

/*
 * RXNE interrupt enable.
 */
#define USART_CR1_RXNEIE  (1U << 5)


#endif /* STM32F103_H */