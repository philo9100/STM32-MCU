#ifndef __DRIVER_ADC_H
#define __DRIVER_ADC_H

#include "stm32f10x.h"

void Driver_ADC1_Init(void);

void Driver_ADC1_DMA_Init();

void Driver_ADC1_DMA_Start(uint32_t dstAddr, uint8_t dataLen);

#endif

