CC = gcc

CFLAGS = -Wall -Wextra -std=c11

ARM_CC = arm-none-eabi-gcc

ARM_CFLAGS = -mcpu=cortex-m3 \
             -mthumb \
             -mfloat-abi=soft \
             -ffreestanding \
             -fno-builtin \
             -Wall \
             -Wextra \
             -std=c11


COMMON_SRC = \
    application/main.c \
    drivers/gpio/gpio.c \
    drivers/uart/uart.c \
    drivers/spi/spi.c \
    drivers/i2c/i2c.c \
    drivers/oled/oled.c \
    drivers/bmp280/bmp280.c \
    interrupt/interrupt.c


STM32_SRC = \
    application/main.c \
    drivers/gpio/gpio.c \
    drivers/uart/uart.c \
    drivers/spi/spi.c \
    drivers/i2c/i2c.c \
    drivers/oled/oled.c \
    drivers/bmp280/bmp280.c \
    interrupt/interrupt.c \
    startup/startup.c


RENODE_TARGET = driver_demo.exe

TEST_TARGET = driver_tests.exe

STM32_TARGET = stm32_driver_framework.elf


.PHONY: all renode stm32 test clean


all: renode stm32


renode:
	$(CC) $(CFLAGS) -DPLATFORM_RENODE=1 $(COMMON_SRC) -o $(RENODE_TARGET)


stm32:
	$(ARM_CC) $(ARM_CFLAGS) $(STM32_SRC) \
	-T startup/linker.ld \
	-nostartfiles \
	-Wl,--gc-sections \
	-Wl,-Map=stm32_driver_framework.map \
	-o $(STM32_TARGET)


test:
	$(CC) $(CFLAGS) \
	tests/test_driver.c \
	drivers/gpio/gpio.c \
	drivers/uart/uart.c \
	drivers/spi/spi.c \
	drivers/i2c/i2c.c \
	drivers/oled/oled.c \
	interrupt/interrupt.c \
	-o $(TEST_TARGET)


clean:
	del /Q $(RENODE_TARGET) $(TEST_TARGET) $(STM32_TARGET) stm32_driver_framework.map 2>NUL