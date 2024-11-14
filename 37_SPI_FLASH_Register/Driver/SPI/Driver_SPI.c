#include "Driver_SPI.h"


/**
 * @brief 通过软件模拟SPI通信
 * @param  
 */
void Driver_SPI_Init(void)
{

    /* 1.开启GPIO时钟 */
    RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN);

    /* 2.设置引脚的工作模式 */
    /* 2.1 CS：推挽输出 PC13 CNF=00 MODE=11 */
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    /* 2.2 SCK：推挽输出 PA5 CNF=00 MODE=11 */
    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    GPIOA->CRL |= GPIO_CRL_MODE5;
    /* 2.3 MOSI：推挽输出 PA7 CNF=00 MODE=11 */
    GPIOA->CRL &= ~GPIO_CRL_CNF7;
    GPIOA->CRL |= GPIO_CRL_MODE7;
    /* 2.4 MISO：浮空输入 PA6 CNF=01 MODE=00 */
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    GPIOA->CRL |= GPIO_CRL_MODE6_0;
    
    /* 3.SPI的模式0：SCK空闲状态是低电平在第一个时钟跳变沿处理一位数据 */
    SCK_LOW;

    /* 4.片选默认不选中 */
    CS_HIGH;

    /* 5.延时 */
    SPI_DELAY;
}


void Driver_SPI_Start(void)
{
    CS_LOW;
    SPI_DELAY;
}


void Driver_SPI_Stop(void)
{
    CS_HIGH;
    SPI_DELAY;
}


uint8_t Driver_SPI_SwapByte(uint8_t byte)
{
    uint8_t i;
    uint8_t rByte = 0x00;   // 存储接收到的数据
    for(i = 0; i < 8; i++)
    {
        /* 1.先把数据放到MOSI上，数据从高位开始 */
        if(byte & 0x80)
        {
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }
        byte <<= 1;
        /* 2.拉高时钟 */
        SCK_HIGH;
        SPI_DELAY;

        /* 3.读取MISO */
        rByte <<= 1;
        if(MISO_READ)
        {
            rByte |= 0x01;
        }

        /* 4.拉低时钟 */
        SCK_LOW;
        SPI_DELAY;
    }

    return rByte;
}


