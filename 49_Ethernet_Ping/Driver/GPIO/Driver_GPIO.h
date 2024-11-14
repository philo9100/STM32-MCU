#ifndef __DRIVER_GPIO_H
#define __DRIVER_GPIO_H

#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"

#define RST_H (GPIOG->ODR |= GPIO_ODR_ODR7)
#define RST_L (GPIOG->ODR &= ~GPIO_ODR_ODR7)


void Driver_GPIO_Init();

#endif

