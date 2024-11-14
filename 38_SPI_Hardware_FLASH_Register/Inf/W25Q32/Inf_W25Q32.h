#ifndef __INF_W25Q32_H
#define __INF_W25Q32_H

#include "Driver_SPI.h"    

void Inf_W25Q32_Init(void);

void Inf_W25Q32_ReadId(uint8_t *mid, uint16_t *did);

void Inf_W25Q32_WriteEnable(void);

void Inf_W25Q32_WriteDisable(void);

void Inf_W25Q32_EraseSector(uint8_t blockNum, uint32_t sectorNum);

void Inf_W25Q32_WritePage(uint8_t blockNum, uint8_t sectorNum, uint8_t pageNum, uint8_t *buff, uint16_t len);

void Inf_W25Q32_Read(uint8_t blockNum, uint8_t sectorNum, uint8_t pageNum, uint8_t *buff, uint16_t len);

#endif

