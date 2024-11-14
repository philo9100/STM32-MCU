#ifndef __INF_W24C02_H
#define __INF_W24C02_H

// #include "Driver_I2C2.h"
#include "string.h"
#include "i2c.h"

#define  ADDR  0xa0 // 从设备地址（EEPROM），或上 0 是写地址，或上 1 是读地址

void Inf_W24C02_Init(void);

void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte);

uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr);

void Inf_W24C02_WritePage(uint8_t innerAddr, uint8_t *page, uint8_t length);

void Inf_W24C02_ReadPage(uint8_t innerAddr, uint8_t *page, uint8_t length);

#endif

