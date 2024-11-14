#include "Driver_TIM6.h"
#include "Delay.h"
/**
 * @description: 给定时器6进行初始化
 * @return {*}
 */
void Driver_TIM6_Init(void)
{
    /* 1. 给定时器6开启时钟*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    /* 2. 设置预分频值: 分频值7199表示7200分频。分频后频率10K，周期100us */
    TIM6->PSC = 7200 - 1;

    /*
    3. 设置自动重转载寄存器的值，决定中断发生的频率。
        假设设置为9999，表示计数器计数10000次发生一次中断。
        计数一次100us，10000次1000000us，正好1s
    */
    TIM6->ARR = 10000 - 1;

    /* 为了避免一启动立即进入更新中断,可以先产生一个更新事件 */
    TIM6->CR1 |= TIM_CR1_URS;
    TIM6->EGR |= TIM_EGR_UG; /* 预分频寄存器和重装载寄存器的值更新到影子寄存器 */
    // TIM6->SR &= ~TIM_SR_UIF;
    
    /* 4. 使能更新中断 */
    TIM6->DIER |= TIM_DIER_UIE;

    /* NVIC配置 */
    /* 5. 设置中断优先级分组 */
    NVIC_SetPriorityGrouping(3);
    /* 6. 设置中断优先级 */
    NVIC_SetPriority(TIM6_IRQn, 1);

    /* 7. 使能定时器6的中断 */
    NVIC_EnableIRQ(TIM6_IRQn);

    /* 8. 使能计数器 */
    TIM6->CR1 |= TIM_CR1_CEN;
}

/**
 * @description: TIM6的中断服务函数
 * @return {*}
 */
void TIM6_IRQHandler(void)
{
    TIM6->SR &= ~TIM_SR_UIF;
    Drviver_LED_Toggle(LED_1);
    printf("1\r\n");
}
