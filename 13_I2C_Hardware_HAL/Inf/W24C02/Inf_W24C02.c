#include "Inf_W24C02.h"

void Inf_W24C02_Init(void)
{
    MX_I2C2_Init();
}

/**
 * @brief 通过I2C写入一个字节
 * @param innerAddr 内存地址
 * @param byte 要写入的字节
 */
void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    /* 
    在 HAL_I2C_Mem_Write 中会调用 I2C_RequestMemoryWrite 函数，其中会有如下判断
     * If Memory address size is 8Bit
        if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
        {
     * Send Memory Address 
            hi2c->Instance->DR = I2C_MEM_ADD_LSB(MemAddress);
        }
    将 innerAddr 即 MemAddress 的值改为 8 位并写入到 hi2c->Instance->DR 中
    */
    HAL_I2C_Mem_Write(&hi2c2, ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 2000);
    HAL_Delay(5);
}

/**
 * @brief 通过I2C读取一个字节
 * @param innerAddr 内存地址
 * @return 读取到的字节
 */
uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
    uint8_t byte = 0;
    /* 从设备（EEPROM）地址要 或上1 才是读操作 */
    HAL_I2C_Mem_Read(&hi2c2, ADDR | 0x01, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 2000);
    return byte;
}

/**
 * @brief 通过I2C发送一个页
 * @param innerAddr 内存地址
 * @param page 要写入的页
 * @param length 要写入的页长度
 */
void Inf_W24C02_WritePage(uint8_t innerAddr, uint8_t *page, uint8_t length)
{
    HAL_I2C_Mem_Write(&hi2c2, ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, page, (uint16_t)length, 2000);
    HAL_Delay(5);
}

/**
 * @brief 通过I2C读取一个页
 * @param innerAddr 内存地址
 * @param page 读取到的页
 * @param length 要读取的页长度
 */
void Inf_W24C02_ReadPage(uint8_t innerAddr, uint8_t *page, uint8_t length)
{
     /* 从设备（EEPROM）地址要 或上1 才是读操作 */
    HAL_I2C_Mem_Read(&hi2c2, ADDR | 0x01, innerAddr, I2C_MEMADD_SIZE_8BIT, page, length, 2000);
}
