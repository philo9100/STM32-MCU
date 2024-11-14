#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#include "stm32f10x.h"

/* 根据原理图，LED1、LED2和LED3对应的分别是PA0、PA1和PA8，三个引脚输出高电平时，熄灭LED灯 */

#define LED_1 GPIO_ODR_ODR0 // (uint16_t)0x0001 对应第一个引脚输出高电平
#define LED_2 GPIO_ODR_ODR1 // (uint16_t)0x0002 对应第二个引脚输出高电平
#define LED_3 GPIO_ODR_ODR8 // (uint16_t)0x0100 对应第九个引脚输出高电平

/* 可以操作Port bit set/reset register 实现原子操作 */
#define LED_1_OFF GPIO_BSRR_BS0 // 使GPIO_ODR_ODR0写入值1 控制PA0引脚输出高电平，熄灭LED1
#define LED_2_OFF GPIO_BSRR_BS1 // 使GPIO_ODR_ODR1写入值1 控制PA1引脚输出高电平，熄灭LED2
#define LED_3_OFF GPIO_BSRR_BS8 // 使GPIO_ODR_ODR8写入值1 控制PA8引脚输出高电平，熄灭LED3
#define LED_1_ON GPIO_BSRR_BR0  // 使GPIO_ODR_ODR0写入值0 控制PA0引脚输出低电平，点亮LED1
#define LED_2_ON GPIO_BSRR_BR1  // 使GPIO_ODR_ODR1写入值0 控制PA1引脚输出低电平，点亮LED2
#define LED_3_ON GPIO_BSRR_BR8  // 使GPIO_ODR_ODR8写入值0 控制PA8引脚输出低电平，点亮LED3

/*
#define LED_1_ON  GPIO_BRR_BR0      // 使GPIO_ODR_ODR0写入值0 控制PA0引脚输出低电平，点亮LED1
#define LED_2_ON  GPIO_BRR_BR1      // 使GPIO_ODR_ODR1写入值0 控制PA1引脚输出低电平，点亮LED2
#define LED_3_ON  GPIO_BRR_BR8      // 使GPIO_ODR_ODR8写入值0 控制PA8引脚输出低电平，点亮LED3
*/

void Driver_LED_Init(void);

void Driver_LED_ON(uint16_t led);

void Driver_LED_OFF(uint16_t led);

void Driver_LED_Toggle(uint16_t led);

void Driver_LED_OnAll(uint16_t leds[], uint8_t size);

void Driver_LED_OffAll(uint16_t leds[], uint8_t size);

#endif
