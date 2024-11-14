#ifndef __DRIVER_SPI_H
#define __DRIVER_SPI_H

#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"

#define CS_HIGH         GPIOD->ODR |= GPIO_ODR_ODR3
#define CS_LOW          GPIOD->ODR &= ~GPIO_ODR_ODR3

#define SPI_DELAY       Delay_us(5)

void Driver_SPI_Init(void);

void Driver_SPI_Start(void);

void Driver_SPI_Stop(void);

uint8_t Driver_SPI_SwapByte(uint8_t byte);

#endif

