#include "Driver_SPI.h"

/**
 * @brief 通过硬件寄存器控制SPI通信
 * @param
 */
void Driver_SPI_Init(void)
{

    /* 1.开启SPI1的时钟 开启GPIO时钟 PA和PC */
    RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN);

    /* 2.设置引脚的工作模式 */
    /* 2.1 CS：推挽输出 PC13 CNF=00 MODE=11 */
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    /* 2.2 SCK：复用推挽输出 PA5 CNF=10 MODE=11 */
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 & ~GPIO_CRL_CNF5_0);
    GPIOA->CRL |= GPIO_CRL_MODE5;
    /* 2.3 MOSI：复用推挽输出 PA7 CNF=10 MODE=11 */
    GPIOA->CRL |= (GPIO_CRL_CNF7_1 & ~GPIO_CRL_CNF7_0 | GPIO_CRL_MODE7);
    /* 2.4 MISO：浮空输入 PA6 CNF=01 MODE=00 */
    GPIOA->CRL |= (~GPIO_CRL_CNF6_1 & GPIO_CRL_CNF6_0 & ~GPIO_CRL_MODE6);
    // GPIOA->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    // GPIOA->CRL |= GPIO_CRL_MODE6_0;

    /* 3.SPI的相关配置 */
    /* 3.1 配置SPI1为主模式 */
    SPI1->CR1 |= SPI_CR1_MSTR;

    /* 3.2 NSS禁用，从设备的片选是使用普通的GPIO控制 */
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR2 &= ~SPI_CR2_SSOE;
    SPI1->CR1 |= SPI_CR1_SSI;

    /* 3.3 配置SPI的工作模式 模式0 时钟的极性和相位 */
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);

    /* 3.4 配置波特率的分频系数 0=2分频 1=4分频 2=8分频 ... */
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_BR_1;

    /* 3.5 配置数据帧的格式：8位 16位 */
    SPI1->CR1 &= ~SPI_CR1_DFF;

    /* 3.6 配置LSB MSB */
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    /* 3.7 使能SPI */
    SPI1->CR1 |= SPI_CR1_SPE;
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
    while (!(SPI1->SR & SPI_SR_TXE))
        ;
    SPI1->DR = byte; // 将数据写到DR寄存器（发送缓冲区）
    /* 1.2 读数据到接收缓冲区，判断接收缓冲区非空 */
    while (!(SPI1->SR & SPI_SR_RXNE))
        ;
    return (uint8_t)(SPI1->DR & 0xFF);
}
