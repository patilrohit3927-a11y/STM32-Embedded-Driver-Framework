# STM32 Embedded Driver Framework

A bare-metal embedded device driver framework developed for the
STM32F103 Cortex-M3 microcontroller and tested using the Renode
hardware simulator.

## Project Overview

This project implements low-level STM32 device drivers without using
an RTOS or HAL library.

The goal is to understand how embedded drivers communicate directly
with memory-mapped hardware registers.

## Target MCU

- STM32F103
- ARM Cortex-M3
- Bare-metal C
- ARM GNU Toolchain

## Simulation

- Renode
- STM32F103 simulation

## Current Drivers

### GPIO
- GPIO clock configuration
- GPIO pin configuration
- GPIO output control
- GPIO pin read
- GPIO toggle

Currently tested using:

- GPIOC
- Pin 13

## Current Status

- [x] STM32F103 Renode simulation
- [x] ARM Cortex-M3 firmware
- [x] Custom startup code
- [x] Custom linker script
- [x] GPIO driver
- [x] GPIO register verification
- [ ] UART driver
- [ ] Interrupt driver
- [ ] SPI driver
- [ ] Driver test framework

## Architecture

Application
    |
    v
Device Drivers
    |
    v
Hardware Abstraction Layer
    |
    v
STM32F103 Memory-Mapped Registers
    |
    v
Renode STM32F103 Simulation

## Tools

- C
- ARM GCC
- Renode
- Git/GitHub
- PowerShell

## Objective

The project is designed to demonstrate practical understanding of:

- Embedded C
- Memory-mapped I/O
- Microcontroller registers
- GPIO peripheral configuration
- Bare-metal programming
- Cortex-M startup
- Linker scripts
- Device driver architecture
- Hardware simulation

## Author

Rohit Patil
