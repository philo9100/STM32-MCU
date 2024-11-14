#include "Driver_TIM1.h"

void Driver_TIM1_Init(void)
{
    /* 1.开启时钟 */
    /* 1.1 TIM1的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    /* 1.2 GPIO的时钟 */
    RCC->APB1ENR |= RCC_APB2ENR_IOPAEN;

    /* 2.给PA8配置复用推挽输出 CNF=10 MODE=11 */
    GPIOA->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8);
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

    /* 3.时基单元的配置 频率是2Hz的PWM */
    /* 3.1 预分频 */
    TIM1->PSC = 7200 - 1;

    /* 3.2 自动重装载寄存器值 */
    TIM1->ARR = 5000 - 1;

    /* 3.3 计数方向 0=向上计数 1=向下计数 */
    TIM1->CR1 &= ~TIM_CR1_DIR;

    /* 3.4 重复寄存器 RCR=4 表示CNT计数器溢出5次，才会产生一个更新事件 */
    TIM1->RCR = 4;

    /* 4.输出部分配置 */
    /* 4.1 把CH1配置为输出 */
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;

    /* 4.2 配置捕获比较寄存器的值 */
    TIM1->CCR1 = 2500; /* 占空比50% */

    /* 4.3 输出的极性 0=高电平有效 1=低电平有效 */ 
    TIM1->CCER &= ~TIM_CCER_CC1P;

    /* 4.4 使能CH1 */
    TIM1->CCER |= TIM_CCER_CC1E;

    /* 4.5 配置CH1通道模式：PWM1=110 */
    TIM1->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;

    /* 4.6 主输出使能（只有高级定时器需要配置） */
    TIM1->BDTR |= TIM_BDTR_MOE;

    /* 4.7 防止一启动就进入更新中断：1.使用UG位产生一个更新事件 2.然后预分频和重复计数器的值更新到影子计数器 */
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->SR &= ~TIM_SR_UIF;

    /* 5.配置中断 */
    /* 5.1 定时器更新中断使能 */
    TIM1->DIER |= TIM_DIER_UIE;

    /* 5.2 NVIC的配置 */
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM1_UP_IRQn, 3);
    NVIC_EnableIRQ(TIM1_UP_IRQn);

}

/**
 * @brief 开启定时器1 
 * @param  
 */
void Driver_TIM1_Generate(void)
{   
    TIM1->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 关闭定时器1
 * 
 */
void Driver_TIM1_Stop(void)
{
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 定时器1的中断服务函数
 * @param  
 */
void TIM1_UP_IRQHandler(void)
{
    if(TIM1->SR & TIM_SR_UIF)
    {
        TIM1->SR &= ~TIM_SR_UIF;
        Driver_TIM1_Stop();
    }

}
