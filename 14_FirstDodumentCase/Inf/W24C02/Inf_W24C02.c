#include "Inf_W24C02.h"

void Inf_W24C02_Init(void)
{
    Driver_I2C2_Init();
}

void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    uint8_t ack;
    /* 1. ��ʼ�ź� */
    ack = Driver_I2C2_Start();
    // printf("start_ack=%d\r\n", ack);
    /* 2. ����д��ַ */
    ack = Driver_I2C_SendAddr(ADDR);
    // printf("addr_ack=%d\r\n", ack);
    /* 4. �����ڲ���ַ */
    ack = Driver_I2C_SendByte(innerAddr);
    // printf("inner_ack=%d\r\n", ack);
    /* 6. ���;������� */
    ack = Driver_I2C_SendByte(byte);
    // printf("byte_ack=%d\r\n", ack);
    /* 8. ֹͣ�ź� */
    Driver_I2C2_Stop();

    Delay_ms(5);
}

uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
    /* 1. ��ʼ�ź� */
    Driver_I2C2_Start();
    /* 2. ����һ��д��ַ   ��д */
    Driver_I2C_SendAddr(ADDR);

    /* 4. �����ڲ���ַ */
    Driver_I2C_SendByte(innerAddr);

    /* 6. ��ʼ�ź� */
    Driver_I2C2_Start();
    /* 7. ���Ͷ���ַ  ��� */
    Driver_I2C_SendAddr(ADDR + 1);

    /* 10. ����һ����Ӧ���ź� */
    Driver_I2C2_NAck();

    /* 11. ֹͣ�ź� */
    Driver_I2C2_Stop();

    /* 9. ��ȡһ���ֽ� */
    uint8_t byte = Driver_I2C_ReadByte();

    return byte;
}

/**
 * @description: ҳд��.һ��д�����ֽ�
 * @param {uint8_t} innerAddr
 * @param {uint8_t} *bytes
 * @param {uint8_t} len
 * @return {*}
 */
void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
    /* 1. ��ʼ�ź� */
    Driver_I2C2_Start();

    /* 2. ����д��ַ */
    Driver_I2C_SendAddr(ADDR);

    /* 4. �����ڲ���ַ */
    Driver_I2C_SendByte(innerAddr);

    for (uint8_t i = 0; i < len; i++)
    {
        /* 6. ���;������� */
        Driver_I2C_SendByte(bytes[i]);
    }
    /* 8. ֹͣ�ź� */
    Driver_I2C2_Stop();

    Delay_ms(5);
}

/**
 * @description: һ���Զ�ȡ����ֽڵ�����
 * @param {uint8_t} innerAddr ��ʼλ��
 * @param {uint8_t} *bytes �洢����������
 * @param {uint8_t} len ��ȡ���ֽ���
 * @return {*}
 */
void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
    /* 1. ��ʼ�ź� */
    Driver_I2C2_Start();

    /* 2. ����һ��д��ַ   ��д */
    Driver_I2C_SendAddr(ADDR);

    /* 4. �����ڲ���ַ */
    Driver_I2C_SendByte(innerAddr);

    /* 6. ��ʼ�ź� */
    Driver_I2C2_Start();

    /* 7. ���Ͷ���ַ  ��� */
    Driver_I2C_SendAddr(ADDR + 1);
    for (uint8_t i = 0; i < len; i++)
    {
        /* 9. ��ȡһ���ֽ� */
        if (i < len - 1)
        {
            Driver_I2C2_Ack();
        }
        else
        {
            Driver_I2C2_NAck();
            Driver_I2C2_Stop();
        }
        /* ����Ӳ�����ԣ�����Ҫ�ȸ�һ��Ӧ����ٶ� */
        bytes[i] = Driver_I2C_ReadByte();
    }
    /* 11. ֹͣ�ź� */
    
}
