#ifndef __INF_W24C02_H
#define __INF_W24C02_H

#include "Driver_I2C2.h"
#include "string.h"

#define ADDR 0xA0
void Inf_W24C02_Init(void);
void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte);

uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr);

void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len);

void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len);

#endif


