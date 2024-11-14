#include "Driver_SPI.h"

/**
 * @brief 通过硬件寄存器控制SPI通信
 * @param
 */
void Driver_SPI_Init(void)
{

    /* 1.开启SPI2的时钟 开启GPIO时钟 PG PB PD */
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    RCC->APB2ENR |= (RCC_APB2ENR_IOPBEN |
                     RCC_APB2ENR_IOPDEN |
                     RCC_APB2ENR_IOPGEN);

    /* 2.设置引脚的工作模式 */
    /* 2.1 CS：推挽输出 PD3 CNF=00 MODE=11 */
    GPIOD->CRL &= ~GPIO_CRL_CNF3;
    GPIOD->CRL |= GPIO_CRL_MODE3;

    /* 2.2 SCK：复用推挽输出 PB13 CNF=10 MODE=11 */
    GPIOB->CRH |= (GPIO_CRH_CNF13_1 & ~GPIO_CRH_CNF13_0);
    GPIOB->CRH |= GPIO_CRH_MODE13;

    /* 2.3 MOSI：复用推挽输出 PB15 CNF=10 MODE=11 */
    GPIOB->CRH |= (GPIO_CRH_CNF15_1 & ~GPIO_CRH_CNF15_0 | GPIO_CRH_MODE15);
    
    /* 2.4 MISO：浮空输入 PB14 CNF=01 MODE=00 */
    GPIOB->CRH |= (~GPIO_CRH_CNF14_1 & GPIO_CRH_CNF14_0 & ~GPIO_CRH_MODE14);

    /* 3.SPI的相关配置 */
    /* 3.1 配置SPI2为主模式 */
    SPI2->CR1 |= SPI_CR1_MSTR;

    /* 3.2 NSS禁用，从设备的片选是使用普通的GPIO控制 */
    SPI2->CR1 |= SPI_CR1_SSM;
    SPI2->CR2 &= ~SPI_CR2_SSOE;
    SPI2->CR1 |= SPI_CR1_SSI;

    /* 3.3 配置SPI的工作模式 模式0 时钟的极性和相位 */
    SPI2->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    /* 3.4 配置波特率的分频系数 0=2分频 1=4分频 2=8分频 ... */
    SPI2->CR1 &= ~SPI_CR1_BR;
    SPI2->CR1 |= SPI_CR1_BR_1;

    /* 3.5 配置数据帧的格式：8位 16位 */
    SPI2->CR1 &= ~SPI_CR1_DFF;

    /* 3.6 配置LSB MSB */
    SPI2->CR1 &= ~SPI_CR1_LSBFIRST;

    /* 3.7 使能SPI */
    SPI2->CR1 |= SPI_CR1_SPE;
}

void Driver_SPI_Start(void)
{
    CS_LOW;
}

void Driver_SPI_Stop(void)
{
    CS_HIGH;
}

uint8_t Driver_SPI_SwapByte(uint8_t byte)
{
    /* 1.写数据到发送缓冲区 */
    /* 1.1 判断发送缓冲区为空 */
    while (!(SPI2->SR & SPI_SR_TXE))
        ;
    SPI2->DR = byte; // 将数据写到DR寄存器（发送缓冲区）
    /* 1.2 读数据到接收缓冲区，判断接收缓冲区非空 */
    while (!(SPI2->SR & SPI_SR_RXNE))
        ;
    return (uint8_t)(SPI2->DR & 0xFF);
}
