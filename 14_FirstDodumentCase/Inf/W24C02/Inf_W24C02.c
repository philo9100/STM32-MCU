#include "Inf_W24C02.h"

void Inf_W24C02_Init(void)
{
    Driver_I2C2_Init();
}

void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    uint8_t ack;
    /* 1. 开始信号 */
    ack = Driver_I2C2_Start();
    // printf("start_ack=%d\r\n", ack);
    /* 2. 发送写地址 */
    ack = Driver_I2C_SendAddr(ADDR);
    // printf("addr_ack=%d\r\n", ack);
    /* 4. 发送内部地址 */
    ack = Driver_I2C_SendByte(innerAddr);
    // printf("inner_ack=%d\r\n", ack);
    /* 6. 发送具体数据 */
    ack = Driver_I2C_SendByte(byte);
    // printf("byte_ack=%d\r\n", ack);
    /* 8. 停止信号 */
    Driver_I2C2_Stop();

    Delay_ms(5);
}

uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
    /* 1. 起始信号 */
    Driver_I2C2_Start();
    /* 2. 发送一个写地址   假写 */
    Driver_I2C_SendAddr(ADDR);

    /* 4. 发送内部地址 */
    Driver_I2C_SendByte(innerAddr);

    /* 6. 起始信号 */
    Driver_I2C2_Start();
    /* 7. 发送读地址  真读 */
    Driver_I2C_SendAddr(ADDR + 1);

    /* 10. 产生一个非应答信号 */
    Driver_I2C2_NAck();

    /* 11. 停止信号 */
    Driver_I2C2_Stop();

    /* 9. 读取一个字节 */
    uint8_t byte = Driver_I2C_ReadByte();

    return byte;
}

/**
 * @description: 页写入.一次写入多个字节
 * @param {uint8_t} innerAddr
 * @param {uint8_t} *bytes
 * @param {uint8_t} len
 * @return {*}
 */
void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
    /* 1. 开始信号 */
    Driver_I2C2_Start();

    /* 2. 发送写地址 */
    Driver_I2C_SendAddr(ADDR);

    /* 4. 发送内部地址 */
    Driver_I2C_SendByte(innerAddr);

    for (uint8_t i = 0; i < len; i++)
    {
        /* 6. 发送具体数据 */
        Driver_I2C_SendByte(bytes[i]);
    }
    /* 8. 停止信号 */
    Driver_I2C2_Stop();

    Delay_ms(5);
}

/**
 * @description: 一次性读取多个字节的数据
 * @param {uint8_t} innerAddr 起始位置
 * @param {uint8_t} *bytes 存储读到的数据
 * @param {uint8_t} len 读取的字节数
 * @return {*}
 */
void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
    /* 1. 起始信号 */
    Driver_I2C2_Start();

    /* 2. 发送一个写地址   假写 */
    Driver_I2C_SendAddr(ADDR);

    /* 4. 发送内部地址 */
    Driver_I2C_SendByte(innerAddr);

    /* 6. 起始信号 */
    Driver_I2C2_Start();

    /* 7. 发送读地址  真读 */
    Driver_I2C_SendAddr(ADDR + 1);
    for (uint8_t i = 0; i < len; i++)
    {
        /* 9. 读取一个字节 */
        if (i < len - 1)
        {
            Driver_I2C2_Ack();
        }
        else
        {
            Driver_I2C2_NAck();
            Driver_I2C2_Stop();
        }
        /* 由于硬件特性，这里要先给一个应答后再读 */
        bytes[i] = Driver_I2C_ReadByte();
    }
    /* 11. 停止信号 */
    
}
