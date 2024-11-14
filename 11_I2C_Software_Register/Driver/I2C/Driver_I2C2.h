#ifndef __DRIVER_I2C2_H
#define __DRIVER_I2C2_H

#include "Delay.h"
#include "stm32f10x.h"
#include "Driver_USART.h"

/* 用于I2C的延时 */
#define I2C2_DELAY_US(us) Delay_us(us)

/* 拉高/拉低 SCL 电平 */
#define I2C2_SCL_HIGH (GPIOB->BSRR |= GPIO_BSRR_BS10)
#define I2C2_SCL_LOW (GPIOB->BSRR |= GPIO_BSRR_BR10)

/* 拉高/拉低 SDA 电平 */
#define I2C2_SDA_HIGH (GPIOB->BSRR |= GPIO_BSRR_BS11)
#define I2C2_SDA_LOW (GPIOB->BSRR |= GPIO_BSRR_BR11)

/* 读取SCL */
#define I2C2_SCL_READ (GPIOB->IDR & GPIO_IDR_IDR10)

/* 读取SDA */
#define I2C2_SDA_READ (GPIOB->IDR & GPIO_IDR_IDR11) 

/* SDA拉低响应 */
#define     ACK     0

/* SDA拉高（默认）非响应 */
#define     NACK    1

void Driver_I2C2_Init(void);

void Driver_I2C2_Start(void);

void Driver_I2C2_Stop(void);

void Driver_I2C2_Ack(void);

void Driver_I2C2_NAck(void);

uint8_t Driver_I2C2_WaitAck(void);

void Driver_I2C2_SendByte(uint8_t byte);

uint8_t Driver_I2C2_ReceiveByte(void);

#endif // __DRIVER_I2C2_H

