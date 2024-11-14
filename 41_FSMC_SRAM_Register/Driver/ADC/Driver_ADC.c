#include "Driver_ADC.h"

/**
 * @brief ADC1的初始化
 * @param
 */
void Driver_ADC1_Init(void)
{
    /* 1.时钟的配置 */
    /* 1.1 使能ADC时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /* 配置ADC的时钟频率不超过14Hz */
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    /* 1.2 GPIO的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* 1.3.GPIO PC0 PC2的工作模式：模拟输入 CNF=00 MODE=00 */
    GPIOC->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    GPIOC->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);

    /* 2.ADC相关配置 */
    /* 2.1 多个通道下开启扫描模式：开启了ADC1的 10通道 和 12通道 */
    ADC1->CR1 |= ADC_CR1_SCAN;

    /* 2.2 启用连续转换模式 CR2 CONT=1 */
    ADC1->CR2 |= ADC_CR2_CONT;

    /* 2.3 数据对齐方式：右对齐 左对齐 */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* 2.4 设置采样时间 ADC_SMPR1 010=13.5 个采样周期 */
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_0);

    ADC1->SMPR1 |= ADC_SMPR1_SMP12_1;
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP12_2 | ADC_SMPR1_SMP12_0);

    /* 2.5 通道组的配置 */
    /* 2.5.1 配置几个通道需要转换 */
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0; // 用于定义常规通道转换序列中的需要转换的通道总数，需要转换两个通道

    /* 2.5.2 把通道号配置到通道组里面 */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1; /* 先SQ1把5位清零 */
    ADC1->SQR3 |= 10 << 0;       /* 设置最后5位，即按常规顺序进行首次转换的通道号 通道10 */
    ADC1->SQR3 &= ~ADC_SQR3_SQ2; /* 先SQ2把5位清零 */
    ADC1->SQR3 |= 12 << 5;       /* 设置SQ2的5位，即按常规顺序进行第二次转换的通道号 通道12 */

    /* 2.6 软件触发 */
    ADC1->CR2 |= ADC_CR2_EXTSEL; /* 选择使用软件触发ADC */

    ADC1->CR2 &= ~ADC_CR2_EXTTRIG; /* 禁用规则组的外部转换 */
}


void Driver_ADC1_DMA_Init()
{
    /* 1.开启DMA时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* 2.设置传输方向：从外设读=0 从内存读=1 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR; // 从ADC1外设读

    /* 3.数据宽度：ADC的数据寄存器是16位，这里配置DMA传输数据宽度为16位 01 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;

    /* 内存与外设数据宽度对齐 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;

    /* 4.外设和内存的地址是否自增：外设地址不增 内存地址要增 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC;
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    /* 5.开启DMA循环传输模式 */
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;

    /* 6.给ADC1开启DMA模式 */
    ADC1->CR2 |= ADC_CR2_DMA;
}


void Driver_ADC1_DMA_Start(uint32_t dstAddr, uint8_t dataLen)
{
    /* 1.DMA配置 */
    DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR)); // 直接将ADC1的DR寄存器的地址作为DMA源地址
    DMA1_Channel1->CMAR = dstAddr;
    DMA1_Channel1->CNDTR = dataLen; // 要传输的数据数（0 到 65535）。按数据宽度记为一个数据
    DMA1_Channel1->CCR |= DMA_CCR1_EN; // 使能DMA

    /****************************************************** */

    /* .ADC1配置 *
    /* 1.上电：把ADC从休眠模式唤醒 */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 2.执行校准 */
    ADC1->CR2 |= ADC_CR2_CAL;

    /* 3.等待校准完成 */
    while (ADC1->CR2 & ADC_CR2_CAL)
        ;

    /* 4.ADON=1 开始转换 0->1 从休眠模式唤醒 1->1 开始转换 */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 5.使用软件开始转换 */
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /* 6.等待首次转换完成 */
    while (!(ADC1->SR & ADC_SR_EOC))
        ;
}






