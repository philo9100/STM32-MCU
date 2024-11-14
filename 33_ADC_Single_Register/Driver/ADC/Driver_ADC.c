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

    /* 1.3.GPIOPC0的工作模式：模拟输入 CNF=00 MODE=00 */
    GPIOC->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);

    /* 2.ADC相关配置 */
    /* 2.1 禁用扫描模式：只有一个通道因此不需要扫描 */
    ADC1->CR1 &= ~ADC_CR1_SCAN;

    /* 2.2 启用连续转换模式 CR2 CONT=1 */
    ADC1->CR2 |= ADC_CR2_CONT;

    /* 2.3 数据对齐方式：右对齐 左对齐 */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* 2.4 设置采样时间 ADC_SMPR1 010=13.5 个采样周期 */
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_0);

    /* 2.5 通道组的配置*/
    /* 2.5.1 配置几个通道需要转换 */
    ADC1->SQR1 &= ~ADC_SQR1_L;

    /* 2.5.2 把通道号配置到通道组里面 */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1; /* 先把5位清零 */
    ADC1->SQR3 |= 10 << 0;       /* 设置最后5位 */

    /* 2.6 软件触发 */
    ADC1->CR2 |= ADC_CR2_EXTSEL; /* 选择使用软件触发ADC */

    ADC1->CR2 &= ~ADC_CR2_EXTTRIG; /* 禁用规则组的外部转换 */
}

void Driver_ADC1_StartConvert(void)
{
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


/**
 * @brief 读取ADC转换后的电压值
 * @param  
 * @return 读取的电压值
 */
double Driver_ADC1_ReadV(void)
{
    /*  */
    return (double)ADC1->DR * 3.3 / 4095;
}





