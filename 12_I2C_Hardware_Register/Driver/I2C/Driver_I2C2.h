#ifndef __DRIVER_I2C2_H
#define __DRIVER_I2C2_H

#include "Delay.h"
#include "stm32f10x.h"
#include "Driver_USART.h"

/* 用于I2C的延时 */
#define I2C2_DELAY_US(us) Delay_us(us)

/* SDA拉低响应 */
#define     ACK     0

/* SDA拉高（默认高阻态）非响应 */
#define     NACK    1

/* 定义函数返回时的状态，执行成功返回OK，执行失败返回FAIL */
#define OK      1
#define FAIL    0

void Driver_I2C2_Init(void);

uint8_t Driver_I2C2_Start(void);

void Driver_I2C2_Stop(void);

void Driver_I2C2_Ack(void);

void Driver_I2C2_NAck(void);

uint8_t Driver_I2C2_SendByte(uint8_t byte);

uint8_t Driver_I2C2_SendAddr(uint8_t addr);

uint8_t Driver_I2C2_ReceiveByte(void);

#endif // __DRIVER_I2C2_H

