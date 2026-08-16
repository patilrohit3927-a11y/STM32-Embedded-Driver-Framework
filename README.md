# STM32 Embedded Driver Framework

A bare-metal embedded device driver framework developed for the
STM32F103 microcontroller and simulated using Renode.

The project demonstrates how low-level peripheral drivers interact
directly with STM32 hardware registers without using an RTOS.

---

## Project Status

Current implementation:

- STM32F103 target
- ARM Cortex-M3
- Bare-metal C
- GPIO driver
- USART1 UART driver
- UART transmit (TX)
- Direct register access
- ARM GCC toolchain
- Renode simulation
- UART output verified in Renode

---

## Project Structure

STM32_Driver_Framework/
│
├── application/
│   └── main.c
│
├── drivers/
│   ├── gpio/
│   │   ├── gpio.c
│   │   └── gpio.h
│   │
│   └── uart/
│       ├── uart.c
│       └── uart.h
│
├── hal/
│   └── stm32f103.h
│
├── interrupt/
│
├── startup/
│   ├── startup.c
│   └── linker.ld
│
├── renode/
│   └── stm32f103.resc
│
├── .gitignore
└── README.md

---

## Hardware Target

Microcontroller:

STM32F103C8T6

CPU:

ARM Cortex-M3

Clock used by the current UART configuration:

8 MHz

UART:

USART1

---

## Software Environment

The project is developed and tested without physical hardware.

### Toolchain

ARM GNU Toolchain

arm-none-eabi-gcc

### Emulator

Renode

Renode is used to simulate the STM32F103 microcontroller and
its peripherals.

---

## Driver Architecture

The project follows a layered embedded driver architecture:

Application
     |
     v
Driver Layer
     |
     +---- GPIO Driver
     |
     +---- UART Driver
     |
     v
Hardware Abstraction
     |
     v
STM32F103 Hardware Registers
     |
     v
Renode STM32F103 Simulation

---

## GPIO Driver

The GPIO driver directly configures STM32 GPIO registers.

Current implementation:

- GPIOC clock enable
- PC13 configuration
- Push-pull output
- GPIO write
- GPIO toggle
- GPIO read

PC13 is used as the GPIO test pin.

---

## UART Driver

USART1 is used for serial communication.

Current implementation:

- USART1 clock enable
- PA9 configured as TX
- PA10 configured as RX
- Baud-rate configuration
- USART enable
- Transmit one byte
- Transmit string
- Receive byte interface

### Current verified feature

UART TX has been successfully verified in Renode.

Test:

```c
UART_SendByte('A');