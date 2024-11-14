#include "Delay.h" // Device header

void Delay_us(uint16_t us)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->PSC = 72 - 1;

    TIM6->ARR = us - 1;

    TIM6->EGR |= TIM_EGR_UG;
    TIM6->SR &= ~TIM_SR_UIF;

    TIM6->CR1 |= TIM_CR1_CEN;

    //while(TIM6->CNT < us - 1);
    while((TIM6->SR & TIM_SR_UIF) == 0);
    TIM6->SR &= ~TIM_SR_UIF;
    TIM6->CR1 &= ~TIM_CR1_CEN;
}

void Delay_ms(uint16_t ms)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->PSC = 7200 - 1;

    TIM6->ARR = 10 * ms - 1;

    TIM6->EGR |= TIM_EGR_UG;
    TIM6->SR &= ~TIM_SR_UIF;

    TIM6->CR1 |= TIM_CR1_CEN;

    //while(TIM6->CNT < us - 1);
    while((TIM6->SR & TIM_SR_UIF) == 0);
    TIM6->SR &= ~TIM_SR_UIF;
    TIM6->CR1 &= ~TIM_CR1_CEN;
}


void Delay_us_1(uint16_t us)
{

    SysTick->LOAD = 72 * us;

    SysTick->VAL = 0;

    SysTick->CTRL = 0x5;

    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE; 
}

void Delay_ms_1(uint16_t ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
}

void Delay_s(uint16_t s)
{
    while (s--)
    {
        Delay_ms(1000);
    }
}
