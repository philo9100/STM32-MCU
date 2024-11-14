#ifndef __DRIVER_DMA_H
#define __DRIVER_DMA_H

#include "stm32f10x.h"
#include "Driver_USART.h"

uint8_t isTransmitFinished = 0; // 中断执行标志位

void Driver_DMA1_Init(void);

void Driver_DMA1_TransimitData(uint32_t srcAddr, uint32_t dstAddr, uint16_t dataLen);

#endif

