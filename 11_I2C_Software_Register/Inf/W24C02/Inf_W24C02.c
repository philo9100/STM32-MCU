#include "Inf_W24C02.h"

void Inf_W24C02_Init(void)
{
    Driver_I2C2_Init();
}

/**
 * @brief 通过I2C写入一个字节
 * @param innerAddr 内存地址
 * @param byte 要写入的字节
 */
void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    /* 1.主设备发送开始信号 */
    Driver_I2C2_Start();

    /* 2.主设备发送从设备地址 写地址*/    
    Driver_I2C2_SendByte(ADDR | 0x00);

    /* 3.主设备等待从设备应答 */
    int8_t ack = Driver_I2C2_WaitAck();
    if (ack == ACK)
    {
        /* 4.发送 EEPROM（从设备）的内存地址 */
        Driver_I2C2_SendByte(innerAddr);

        /* 5.主设备等待从设备应答 */
        Driver_I2C2_WaitAck();

        /* 6.主设备发送具体数据 */
        Driver_I2C2_SendByte(byte);

        /* 7.主设备等待从设备应答 */
        Driver_I2C2_WaitAck();

        /* 8.停止信号 */
        Driver_I2C2_Stop();
    }
    Delay_ms(5);
}

/**
 * @brief 通过I2C读取一个字节
 * @param innerAddr 内存地址
 * @return 读取到的字节
 */
uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
    /* 1.主设备发送开始信号 */
    Driver_I2C2_Start();

    /* 2.主设备发送从设备地址 写地址 根据EEPROM手册这里是设计的假写*/    
    Driver_I2C2_SendByte(ADDR | 0x00);

    /* 3.主设备等待从设备应答 */
    int8_t ack = Driver_I2C2_WaitAck();
    if (ack == ACK)
    {
        /* 4.发送 EEPROM（从设备）的内存地址 */
        Driver_I2C2_SendByte(innerAddr);

        /* 5.主设备等待从设备应答 */
        Driver_I2C2_WaitAck();

        /* 6.主设备发送开始信号 */
        Driver_I2C2_Start();

        /* 7.主设备发送从设备地址 读地址 这里是真读取EEPROM内存中的数据 */
        Driver_I2C2_SendByte(ADDR | 0x01);

        /* 8.等待应答 */
        Driver_I2C2_WaitAck();

        /* 9.读取一个字节 */
        uint8_t byte = Driver_I2C2_ReceiveByte();

        /* 10.从设备一个非应答信号 */
        Driver_I2C2_NAck();

        /* 11.停止信号 */
        Driver_I2C2_Stop();

        return byte;
    }

    return 0;
}

/**
 * @brief 通过I2C发送一个页
 * @param innerAddr 内存地址
 * @param page 要写入的页
 * @param length 要写入的页长度
 */
void Inf_W24C02_WritePage(uint8_t innerAddr, uint8_t *page, uint8_t length)
{
    /* 1.主设备发送开始信号 */
    Driver_I2C2_Start();

    /* 2.主设备发送从设备地址 写地址*/    
    Driver_I2C2_SendByte(ADDR | 0x00);

    /* 3.主设备等待从设备应答 */
    int8_t ack = Driver_I2C2_WaitAck();
    if (ack == ACK)
    {
        /* 4.发送 EEPROM（从设备）的内存地址 */
        Driver_I2C2_SendByte(innerAddr);

        /* 5.主设备等待从设备应答 */
        Driver_I2C2_WaitAck();

        /* 6.主设备发送具体数据 */
        for (uint8_t i = 0; i < length; i++)
        {
            Driver_I2C2_SendByte(page[i]);

            /* 7.主设备等待从设备应答 */
            Driver_I2C2_WaitAck();    
        }
        
        /* 8.停止信号 */
        Driver_I2C2_Stop();
    }
    Delay_ms(5);
} 


/**
 * @brief 通过I2C读取一个页 
 * @param innerAddr 内存地址
 * @param page 读取到的页
 * @param length 要读取的页长度
 */
void Inf_W24C02_ReadPage(uint8_t innerAddr, uint8_t *page, uint8_t length)
{
    /* 1.主设备发送开始信号 */
    Driver_I2C2_Start();

    /* 2.主设备发送从设备地址 写地址 根据EEPROM手册这里是设计的假写*/    
    Driver_I2C2_SendByte(ADDR | 0x00);

    /* 3.主设备等待从设备应答 */
    int8_t ack = Driver_I2C2_WaitAck();
    if (ack == ACK)
    {
        /* 4.发送 EEPROM（从设备）的内存地址，读取数据的起始地址 */
        Driver_I2C2_SendByte(innerAddr);

        /* 5.主设备等待从设备应答 */
        Driver_I2C2_WaitAck();

        /* 6.主设备发送开始信号 */
        Driver_I2C2_Start();

        /* 7.主设备发送从设备地址 读地址 这里是真读取EEPROM内存中的数据 */
        Driver_I2C2_SendByte(ADDR | 0x01);

        /* 8.等待应答 */
        Driver_I2C2_WaitAck();

        /* 连续读取一个页 */
        for (uint8_t i = 0; i < length; i++)
        {
            /* 9.读取一个字节 */
            page[i] = Driver_I2C2_ReceiveByte();
            
            // if (i == length - 1)
            // {
            //     /* 10.根据时序图在读取完最后一个字节后，从设备有一个非应答信号 */
            //     Driver_I2C2_NAck();
            // } else {
            //     Driver_I2C2_Ack();
            // }
            
            i == length - 1 ? Driver_I2C2_NAck() : Driver_I2C2_Ack();

        }

        /* 11.停止信号 */
        Driver_I2C2_Stop();
    }
} 



