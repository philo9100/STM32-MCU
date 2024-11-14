#include "Driver_TIM3.h"

void Driver_TIM3_Init(void)
{
    /* 1.  开启时钟*/
    /* 1.1 定时器3的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    /* 1.2 GPIO的时钟 PA */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2. 设置GPIO的复用推挽输出 PB0 CNF=10 MODE=11*/
    GPIOB->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0);
    GPIOB->CRL &= ~GPIO_CRL_CNF0_0;

    /* 3. 定时器配置 */
    /* 3.1 预分频器的配置 */
    TIM3->PSC = 72 - 1;
    /* 3.2 自动重装载寄存器的配置 */
    TIM3->ARR = 100 - 1;
    /* 3.3 计数器的计数方向 0=向上 1=向下*/
    TIM3->CR1 &= ~TIM_CR1_DIR;
    /* 3.4 配置通道3的捕获比较寄存器 */
    TIM3->CCR3 = 97;
    /* 3.5 把通道3配置为输出  CCMR1_CC2S=00 */
    TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;
    /* 3.6 配置通道的输出比较模式 CCMR1_OC2M=110*/
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    /* 3.7 使能通道3  CCER_CC2E=1 */
    TIM3->CCER |= TIM_CCER_CC3E;

    /* 3.8 设置通道的极性 0=高电平有效  1=低电平有效 */
    TIM3->CCER &= ~TIM_CCER_CC3P;
}

void Driver_TIM3_Start(void)
{
    /* 使能计数器 */
    TIM3->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM3_Stop(void)
{
    /* 失能计数器 */
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void Driver_TIM3_SetDutyCycle(uint8_t dutyCycle)
{
    TIM3->CCR3 = dutyCycle;
}