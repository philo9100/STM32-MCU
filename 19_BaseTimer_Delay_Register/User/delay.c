#include "delay.h"

void Delay_us_BaseTimer(uint16_t us)
{
    /* 开启定时器时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    /* 配置预分频值 1us 计数一次 */
    TIM6->PSC = 72 - 1;

    /* 配置重装载值 计数多少个us */
    TIM6->ARR = us - 1;

    /* 先更新中断将值写入影子寄存器 */
    TIM6->EGR |= TIM_EGR_UG;
    TIM6->SR &= ~TIM_SR_UIF;

    /* 启动定时器 */
    TIM6->CR1 |= TIM_CR1_CEN;

    /* 方式一 判断计数器是否溢出 */
    // while (TIM6->CNT < us - 1);
    /* 方式二 判断是否产生更新中断 中断标志位 */    
    while ((TIM6->SR & TIM_SR_UIF) == 0);
    TIM6->SR &= ~TIM_SR_UIF;

    /* 关闭定时器 */
    TIM6->CR1 &= ~TIM_CR1_CEN;
}

void Delay_ms_BaseTimer(uint16_t ms)
{
    /* 开启定时器时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    /* 配置预分频值 0.1ms 计数一次 */
    TIM6->PSC = 7200 - 1;

    /* 配置重装载值 计数多少个 10 * 0.1ms */
    TIM6->ARR = 10 * ms - 1;

    /* 先更新中断将值写入影子寄存器 */
    TIM6->EGR |= TIM_EGR_UG;
    TIM6->SR &= ~TIM_SR_UIF;

    /* 启动定时器 */
    TIM6->CR1 |= TIM_CR1_CEN;

    /* 方式一 判断计数器是否溢出 */
    // while (TIM6->CNT < us - 1);
    /* 方式二 判断是否产生更新中断 中断标志位 */    
    while ((TIM6->SR & TIM_SR_UIF) == 0);
    TIM6->SR &= ~TIM_SR_UIF;

    /* 关闭定时器 */
    TIM6->CR1 &= ~TIM_CR1_CEN;
}



void Delay_us(uint16_t us)
{
    /* 定时器重装值 */
    /* 使用SysTick Timer作为计时器，工作频率设置为72MHz 
    即1s滴答72000000次，1ms滴答72000次，1us滴答72次
    执行us次后，将定时器重装减为零后不需要重新装载，因此，这里不需要减1
    */
    SysTick->LOAD = us * 72;

    /* 清除当前计数值 */
    SysTick->VAL = 0x00;

    /* 设置内部时钟源（2位->1），不需要中断（1位->0），并启动定时器（0位->1）*/
    SysTick->CTRL = 0x05;

    /* 等待计数值（LOAD中的值）为0，如果计数到0则 SysTick->CTRL 寄存器的16位会置为1 */
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG));

    /* 关闭定时器 */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

void Delay_ms(uint16_t ms)
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
