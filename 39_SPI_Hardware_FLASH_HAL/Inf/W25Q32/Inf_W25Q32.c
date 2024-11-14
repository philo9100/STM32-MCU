#include "Inf_W25Q32.h"

void Inf_W25Q32_Init(void)
{
    MX_SPI1_Init();
}

void Inf_W25Q32_ReadId(uint8_t *mid, uint16_t *did)
{

    Driver_SPI_Start();

    /* 1.发送 JEDEC Id 指令 */
    /*
    其中0x9f是主设备通过SPI给从设备发送的指令，从设备（Flash）接收到指令并能够识别该指令（该指令是从设备内部的指令）
    从设备在读取指令数据后会通过SPI返回数据给主设备，返回的数据可以通过查询从设备的手册了解
    */
    HAL_SPI_Transmit(&hspi1, &(uint8_t){0x9f}, 1, 1000);

    /* 2.获取厂商id */
    uint8_t buff[3] = {0};

    HAL_SPI_Receive(&hspi1, buff, 3, 1000);

    *mid = buff[0];
    *did = (buff[1] << 8) | buff[2];

    Driver_SPI_Stop();
}

/**
 * @brief 读取FLASH内部状态寄存器的BUSY位，判断FLASH是否处于忙状态
 * @param
 */
void Inf_W25Q32_isBusy(void)
{
    Driver_SPI_Start();

    HAL_SPI_Transmit(&hspi1, &(uint8_t){0x05}, 1, 1000); // 发送读取状态寄存器BUSY位的指令

    uint8_t buff;
    do
    {
        HAL_SPI_Receive(&hspi1, &buff, 1, 1000);
    } while (buff & 0x01); // 判断busy位

    Driver_SPI_Stop();
}

/**
 * @brief FLASH写使能 写使能指令0x06
 * @param
 */
void Inf_W25Q32_WriteEnable(void)
{
    Driver_SPI_Start();
    HAL_SPI_Transmit(&hspi1, &(uint8_t){0x06}, 1, 1000);
    Driver_SPI_Stop();
}

/**
 * @brief FLASH写禁止 写禁止指令0x04
 * @param
 */
void Inf_W25Q32_WriteDisable(void)
{
    Driver_SPI_Start();
    HAL_SPI_Transmit(&hspi1, &(uint8_t){0x04}, 1, 1000);
    Driver_SPI_Stop();
}

void Inf_W25Q32_EraseSector(uint8_t blockNum, uint32_t sectorNum)
{
    uint32_t startAddr = 0;

    if (blockNum < 0x00 ||
        blockNum > 0x3f ||
        sectorNum < 0x00 ||
        sectorNum > 0x0f)
    {
        return;
    }

    startAddr |= (blockNum << 16 | sectorNum << 12);

    startAddr &= 0xfff000;

    Inf_W25Q32_isBusy();

    /* 擦除之前要写使能，要写入擦除指令 */
    Inf_W25Q32_WriteEnable();

    Driver_SPI_Start();
    HAL_SPI_Transmit(&hspi1, &(uint8_t){0x20}, 1, 1000); // 主设备发送擦除扇区指令 0x20

    // Driver_SPI_SwapByte((startAddr >> 16) & 0xff); // 主设备发送要擦除FLASH的起始地址高8位，抛弃高位
    // Driver_SPI_SwapByte((startAddr >> 8) & 0xff);  // 主设备发送要擦除FLASH的起始地址中间8位，抛弃高位
    // Driver_SPI_SwapByte(startAddr & 0xff);         // 主设备发送要擦除FLASH的起始地址低8位，抛弃高位

    HAL_SPI_Transmit(&hspi1,
                     &(uint8_t[]){{(startAddr >> 16) & 0xff, (startAddr >> 8) & 0xff, startAddr & 0xff}},
                     3,
                     1000);

    Inf_W25Q32_WriteDisable();

    Driver_SPI_Stop();
}

void Inf_W25Q32_WritePage(uint8_t blockNum,
                          uint8_t sectorNum,
                          uint8_t pageNum,
                          uint8_t *data,
                          uint16_t len)
{
    uint32_t startAddr = 0;

    if (blockNum < 0x00 ||
        blockNum > 0x3f ||
        sectorNum < 0x00 ||
        sectorNum > 0x0f ||
        pageNum < 0x00 ||
        pageNum > 0x0f)
    {
        return;
    }

    startAddr |= (blockNum << 16 | sectorNum << 12 | pageNum << 8);

    startAddr &= 0xffff00;

    Inf_W25Q32_isBusy();

    /* 先写使能 */
    Inf_W25Q32_WriteEnable();

    Driver_SPI_Start();
    HAL_SPI_Transmit(&hspi1, &(uint8_t){0x02}, 1, 1000); // 主设备发送写入页指令 0x02

    // Driver_SPI_SwapByte((startAddr >> 16) & 0xff); // 主设备发送要写入FLASH页的起始地址高8位，抛弃高位
    // Driver_SPI_SwapByte((startAddr >> 8) & 0xff);  // 主设备发送要写入FLASH页的起始地址中间8位，抛弃高位
    // Driver_SPI_SwapByte(startAddr & 0xff);         // 主设备发送要写入FLASH页的起始地址低8位，抛弃高位

    HAL_SPI_Transmit(&hspi1,
                     &(uint8_t[]){{(startAddr >> 16) & 0xff, (startAddr >> 8) & 0xff, startAddr & 0xff}},
                     3,
                     1000);

    uint16_t i;

    /* 要发送的数据 */
    HAL_SPI_Transmit(&hspi1, data, len, 1000);

    Inf_W25Q32_WriteDisable();

    Driver_SPI_Stop();
}

void Inf_W25Q32_Read(uint8_t blockNum,
                     uint8_t sectorNum,
                     uint8_t pageNum,
                     uint8_t *buff,
                     uint16_t len)
{
    
    if (blockNum < 0x00 ||
        blockNum > 0x3f ||
        sectorNum < 0x00 ||
        sectorNum > 0x0f ||
        pageNum < 0x00 ||
        pageNum > 0x0f)
    {
        return;
    }

    uint32_t startAddr = 0;

    startAddr |= (blockNum << 16 | sectorNum << 12 | pageNum << 8);

    startAddr &= 0xffff00;

    Inf_W25Q32_isBusy();

    /* 先写使能 */
    Inf_W25Q32_WriteEnable();

    Driver_SPI_Start();

   HAL_SPI_Transmit(&hspi1, &(uint8_t){0x03}, 1, 1000); // 主设备发送读指令 0x03

    // Driver_SPI_SwapByte((startAddr >> 16) & 0xff); // 主设备发送要读取FLASH的起始地址高8位，抛弃高位
    // Driver_SPI_SwapByte((startAddr >> 8) & 0xff);  // 主设备发送要读取FLASH的起始地址中间8位，抛弃高位
    // Driver_SPI_SwapByte(startAddr & 0xff);         // 主设备发送要读取FLASH的起始地址低8位，抛弃高位

    HAL_SPI_Transmit(&hspi1,
                     &(uint8_t[]){{(startAddr >> 16) & 0xff, (startAddr >> 8) & 0xff, startAddr & 0xff}},
                     3,
                     1000);

    HAL_SPI_Receive(&hspi1, buff, len, 1000);
    
    Inf_W25Q32_WriteDisable();

    Driver_SPI_Stop();
}
