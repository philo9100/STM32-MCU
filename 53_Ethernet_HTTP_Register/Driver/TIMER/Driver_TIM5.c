#include "Driver_TIM5.h"


void Driver_TIM5_Init(void)
{
    /* 1.开启时钟 */
    /* 1.1 开启定时器TIM5的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

    /* 1.2 GPIOA的时钟 PA */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2.设置GPIOPA1的工作模式 复用为通用定时器TIM5的2通道 推挽输出 CNF=10 MODE=11 */
    GPIOA->CRL |= GPIO_CRL_CNF1_1 | GPIO_CRL_MODE1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    /* 3.通用定时器TIM5配置 */
    /* 3.1 计数器的预分频器的预分频值 分频7200次 1s计数10000次 */
    TIM5->PSC = 7200 - 1;

    /* 3.2 配置自动重装载寄存器 计数100次 则周期为0.01s */
    TIM5->ARR = 100 - 1;

    /* 3.3 配置计数器的计数方向 0=向上 1=向下 */
    TIM5->CR1 &= ~TIM_CR1_DIR;

    /* 3.4 配置TIM5通道2的捕获比较寄存器 由值决定占空比 这里占空比配置为50% */
    TIM5->CCR2 = 50;

    /* 3.5 配置通道2为输出模式 CCMR1_CC2S = 00 */
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;

    /* 3.6 配置通道2的输出比较模式 CCMR1_OC2M = 110 */
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;

    /* 3.7 使能通道2 */
    TIM5->CCER |= TIM_CCER_CC2E;

    /* 3.8 设置通道的极性 0=高电平有效 1=低电平有效 */
    TIM5->CCER &= ~TIM_CCER_CC2P;
}

void Driver_TIM5_Generate(void)
{   
    /* 使能更新事件 */
    TIM5->EGR |= TIM_EGR_UG;

    /* 清除更新标志位 */
    TIM5->SR &= ~TIM_SR_UIF;

    /* 使能定时器 */
    TIM5->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM5_Stop(void)
{
    /* 禁止定时器 */
    TIM5->CR1 &= ~TIM_CR1_CEN;
}


/**
 * @brief 修改TIM5CH2的占空比
 * @param dutyCycle 占空比
 */
void Driver_TIM5_SetDutyCycle(uint8_t dutyCycle)
{
    TIM5->CCR2 = dutyCycle;
}
