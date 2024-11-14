#include "Driver_DMA.h"

void Driver_DMA1_Init(void)
{
    /* 1.开启DMA时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* 2.DMA相关的配置 */
    /* 2.1 DMA方向：从存储器（ROM）到存储器（RAM）任意通道 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR; // 0：从外设读 1：从存储器读
    DMA1_Channel1->CCR |= DMA_CCR1_MEM2MEM;

    /* 2.2 设置存取器和外设的数据宽度：8位（一个字节）16位（半字）32位（字） */
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE; // 00：8位 01：16位 10：32位 11：保留
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE;

    /* 2.3 设置外设和存储器的地址是否自增 */
    DMA1_Channel1->CCR |= DMA_CCR1_PINC; // 0：外设地址固定 1：自增
    DMA1_Channel1->CCR |= DMA_CCR1_MINC; // 0：存储器地址固定 1：自增

    /* 2.4 开启传输完成的中断 */
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE;

    /* 2.5 NVIC的设置 */
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/**
 * @brief 启动DMA传输数据
 * @param srcAddr 源地址
 * @param dstAddr 目的地址
 * @param datalen 要传输的数据长度
 */
void Driver_DMA1_TransimitData(uint32_t srcAddr, uint32_t dstAddr, uint16_t dataLen)
{
    /* 1.设置外设地址 */
    DMA1_Channel1->CPAR = srcAddr;

    /* 2.设置存储器地址 */
    DMA1_Channel1->CMAR = dstAddr;

    /* 3.设置要传输的数据量 */
    DMA1_Channel1->CNDTR = dataLen;

    /* 4.开启通道，开始传输 */
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}


/**
 * @brief DMA1_Channel1中断服务函数
 * @param
 */
void DMA1_Channel1_IRQHandler(void)
{
    /* 判断是否是传输完成中断 */
    if (DMA1->ISR & DMA_ISR_TCIF1)
    {
        /* 清除传输完成中断标志位 */
        DMA1->IFCR |= DMA_IFCR_CGIF1;

        /* 设置中断执行标志位：DMA传输完成 */
        isTransmitFinished = 1;

        /* 通道不再需要，可以关闭 */
        DMA1_Channel1->CCR &= ~DMA_CCR1_EN;
    }

}
