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
    /* 4.1 TI1的选择：把CH1直接连接到TI1 */
    TIM4->CR2 &= ~TIM_CR2_TI1S;

    /* 4.2 信号的输入滤波器：不滤波 CCMR1_IC1F=0000 */
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;

    /* 4.3 配置边沿检测器：上升(0)/下降(1)沿捕获 CCER_CC1P=0 */
    TIM4->CCER &= ~TIM_CCER_CC1P;

    /* 4.4 CH1配置为输入，并把信号映射到IC1 CC1S=01 */
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;

    /* 4.5 对信号做分频（针对信号频率比较高的时候）CCMR1_IC1PSC=00 不分频 01 2分频 10 4分频 11 8分频 */
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;

    /* 4.6 CH1输入捕获使能 CCER_CC1E=1 */
    TIM4->CCER |= TIM_CCER_CC1E;

    /* 4.7 开启捕获中断 DIER_CC1IE=1 */
    TIM4->DIER |= TIM_DIER_CC1IE;

    /* 5.NVIC的配置 */
    /* 5.1 优先级组 */
    NVIC_SetPriorityGrouping(3);
    /* 5.2 优先级 */
    NVIC_SetPriority(TIM4_IRQn, 3);
    /* 5.3 使能中断 */
    NVIC_EnableIRQ(TIM4_IRQn);

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


uint8_t raisedEdgeCount = 0;
uint16_t t = 0;
/**
 * @brief TIM4的中断服务函数
 * @param  
 */
void TIM4_IRQHandler(void)
{
    /* 判断是否是CH1发生了捕获中断 */
    if (TIM4->SR & TIM_SR_CC1IF)
    {
        /* 清除中断标志位 */
        TIM4->SR &= ~TIM_SR_CC1IF;

        /* 如果是第1个上升沿，则清零计数器，让计数器从0开始计数 */
        raisedEdgeCount++;
        if (raisedEdgeCount == 1)
        {
            TIM4->CNT = 0;  /* 计数器清零 */
        } else if (raisedEdgeCount == 2)
        {
            /* 读取捕获寄存器的值，就是周期 */
            t = TIM4->CCR1;

            /* 上升沿的计数从0重新计数 */
            raisedEdgeCount = 0;
        }
    }
}


/**
 * @brief 返回PWM的周期
 * @param  
 * @return 返回ms
 */
double Driver_TIM4_GetPWMCycle(void)
{
    return t / 1000.0;
}

/**
 * @brief 返回PWM的频率
 * @param  
 * @return 返回s
 */
double Driver_TIM4_GetPWMFrequency(void)
{
    return 1000000 / t;
}