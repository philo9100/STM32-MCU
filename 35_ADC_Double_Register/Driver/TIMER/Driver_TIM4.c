#include "Driver_TIM4.h"

void Driver_TIM4_Init(void)
{
    /* 1.开启时钟 */
    /* 1.1 开启定时器TIM4的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    /* 1.2 GPIOB的时钟 PB6 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2.设置GPIOPB6的工作模式 浮空输入 CNF=01 MODE=00 */
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);    
    GPIOB->CRL |= GPIO_CRL_CNF6_0;

    /* 3.配置定时器的时基部分 */
    /* 3.1 计数器的预分频器的预分频值 分频72次 1s计数1000000次 1us计一次数 */
    TIM4->PSC = 72 - 1;

    /* 3.2 配置自动重装载寄存器 计数655535次 设置最大 */
    TIM4->ARR = 65536 - 1;

    /* 3.3 配置计数方向 向上计数 */
    TIM4->CR1 &= ~TIM_CR1_DIR;

    /* 4.输入捕获部分 */
    /* 4.1 输入信号不滤波 */
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;

    /* 4.2 配置边沿检测：去IC1上升沿 去IC2下降沿 */    
    TIM4->CCER &= ~TIM_CCER_CC1P;
    TIM4->CCER |= TIM_CCER_CC2P;

    /* 4.3 把IC1映射到TI1 CCMR1_CC1S=01 上升沿 */
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;

    /* 4.4 把IC2映射到TI1 CCMR1_CC2S=10 下降沿*/    
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;
    
    /* 4.5 IC1和IC2的预分频 不分频 */
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2PSC;

    /* 4.6 配置TRGI信号：TI1FP1 SMCR_TS=101 */
    TIM4->SMCR |= (TIM_SMCR_TS_2 | TIM_SMCR_TS_0);
    TIM4->SMCR &= ~TIM_SMCR_TS_1;
    
    /* 4.7 配置从模式：为复位模式 SMCR_SMS=100 */
    TIM4->SMCR |= TIM_SMCR_SMS_2;
    TIM4->SMCR &= ~(TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1);

    /* 4.8 使能CH1捕获和CH2捕获 */
    TIM4->CCER |= TIM_CCER_CC1E;
    TIM4->CCER |= TIM_CCER_CC2E;

}

void Driver_TIM4_Generate(void)
{   
    /* 使能更新事件 */
    TIM4->EGR |= TIM_EGR_UG;

    /* 清除更新标志位 */
    TIM4->SR &= ~TIM_SR_UIF;

    /* 使能定时器 */
    TIM4->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM4_Stop(void)
{
    /* 禁止定时器 */
    TIM4->CR1 &= ~TIM_CR1_CEN;
}


/**
 * @brief 返回PWM的周期
 * @param  
 * @return 返回ms
 */
double Driver_TIM4_GetPWMCycle(void)
{
    return TIM4->CCR1 / 1000.0;
}

/**
 * @brief 返回PWM的频率
 * @param  
 * @return 返回s
 */
double Driver_TIM4_GetPWMFrequency(void)
{
    return 1000000.0 / TIM4->CCR1;
}


/**
 * @brief 用高电平的时间除以PWM的周期就是占空比
 * @return 返回占空比
 */
double Driver_TIM4_GetDutyCycle(void)
{
    /* 将数值转为浮点数 */
    return TIM4->CCR2 * 1.0 / TIM4->CCR1;
}
