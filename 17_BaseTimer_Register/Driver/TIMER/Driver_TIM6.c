#include "Driver_TIM6.h"

/**
 * STM32F103ZE的基本定时器（TIM6和TIM7）从APB1总线获得时钟源。
 * APB1的默认时钟频率可以通过主系统时钟（SYSCLK）和APB1分频因子（PCLK1）来确定。
 *
 * 基本定时器 计数器时钟计数频率 (f(tim)) 计算如下
 * f(tim) = f(apb1) / apb1_prescaler(APB1预分频器)
 *
 * 当APB1预分频器大于1时，定时器时钟会被再乘以2，总之，TIM6（基本定时器）的时钟频率为 72MHz
 *
 * 计数器的周期（计数器多久会溢出一次）由计数器的预分频器（PSC, Prescaler）和自动重装载寄存器（ARR, Auto-reload Register）的值决定。
 *
 * T = (ARR + 1) / f(tim) / (PSC + 1)
 *
 * 例如，计数器时钟频率为f(tim) = 72MHz
 * 定时器的预分频器（PSC, Prescaler）的值为 7199 （从0开始到7199，分频7200次）
 * 自动重装载寄存器（ARR, Auto-reload Register）的值为 9999 （从0开始到9999，重装载10000次）
 *
 * 则定时时间 T 为
 *
 * T = (9999 + 1) * (7199 + 1) * (1/72) * 10^-6 = 1s
 *
 * 将计数器时钟频率f(tim)由原来的 72MHz 次（即 1s 72M次）降低为 10000 次（即 1s 10000次）
 * 那么重装载值设置为 10000次计数完就是 1s
 *
 * @brief 给定时器6进行初始化
 * @param
 */
void Driver_TIM6_Init(void)
{
    /* 1.给定时器6开启时钟，使能TIM6的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    /* 2.设置TIM6的计数器的预分频器的预分频值，计时器的时钟频率为72MHz，降频为 10KHz，那么要分频7200次
        这个预分频值只有在产生一次更新中断后才能写入到预分频器的影子寄存器中
        因此Tim6启动前的时候如果不进行手动的产生一次更新中断，那么这个预分频值就不会被写入到影子寄存器中
        Tim6启动时会先以72MHz计数器的频率计数10000次产生一次更新中断，这不符合1s产生一次更新中断的要求
    */
    TIM6->PSC = 7200 - 1; // 从0开始

    /* 设置TIM6的自动重装载寄存器，定时周期为1秒，那么要计数 10K 次 */
    TIM6->ARR = 10000 - 1; // 从0开始

    /* 使能TIM6的更新事件（计数器每溢出一次硬件会触发一次更新事件） */
    /*
    URS: Update request source
    This bit is set and cleared by software to select the UEV event sources.
    0: Any of the following events generates an update interrupt or DMA request if enabled.
    These events can be:
    – Counter overflow/underflow
    – Setting the UG bit
    – Update generation through the slave mode controller

    1: Only counter overflow/underflow generates an update interrupt or DMA request if enabled
    */
    TIM6->CR1 |= TIM_CR1_URS;

    /* 为了避免定时器一启动不到1s就立即进入更新中断，可以先手动产生一个更新事件，将计时器的预分频器的预分频值从自动重装载寄存器的预加载寄存器写入到影子寄存器（实际的自动重装载寄存器） */
    TIM6->EGR |= TIM_EGR_UG; // 这个执行过后会先将计时器的预分频值写于影子寄存器，按照自定义的预分频值的频率进行计数
                             // 否则计数器开始的时候会按照72MHz的频率计数10000次，不到1s产生一次计数器溢出触发更新事件才将预分频值写入影子寄存器

    /* 由于手动触发了一次更新事件虽然还没有使能更新中断，SR寄存器的 UIF 中断挂起标志位会会置为1，这里还没有使能更新中断执行中断服务函数用来清除该位，因此要手动清除 */
    TIM6->SR &= ~TIM_SR_UIF;

    /* 3.使能TIM6的更新中断 */
    TIM6->DIER |= TIM_DIER_UIE;

    /*
    在STM32F103的定时器TIM6中，CR1寄存器的URS位和DIER寄存器的UIE位都与更新中断相关。

    - URS（Update Request Source）位是CR1寄存器的一位，用于配置更新事件的触发源。当URS位被设置为1时，只有当发生更新事件时，才会产生更新中断。如果URS位被清除为0，则无论何时发生更新事件，都会产生更新中断。
        URS位的作用是限制更新中断的触发条件。

    - UIE（Update Interrupt Enable）位是DIER寄存器的一位，用于使能或禁用更新中断。当UIE位被设置为1时，允许更新中断的产生。如果UIE位被清除为0，则禁止更新中断的产生。
        UIE位的作用是控制是否允许更新中断的触发。
    */

    /* 配置NVIC */
    /* 5.设置中断优先级组 */
    NVIC_SetPriorityGrouping(3);

    /* 6.设置中断优先级 */
    NVIC_SetPriority(TIM6_IRQn, 1);

    /* 7.使能定时器6的中断 */
    NVIC_EnableIRQ(TIM6_IRQn);

    /* 启动TIM6 */
    /*
    CEN: Counter enable
    0: Counter disabled
    1: Counter enabled
    Note: Gated mode can work only if the CEN bit has been previously set by software. However
    trigger mode can set the CEN bit automatically by hardware.
    CEN is cleared automatically in one-pulse mode, when an update event occurs
    */
    TIM6->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 基本定时器6的中断服务函数
 * @param
 *
 */
void TIM6_IRQHandler(void)
{
    /* 先清除中断标志位 */
    /*
    UIF: Update interrupt flag
    This bit is set by hardware on an update event. It is cleared by software.
    
    0: No update occurred.
    1: Update interrupt pending. This bit is set by hardware when the registers are updated:
    – At overflow or underflow and if UDIS = 0 in the TIMx_CR1 register.
    – When CNT is reinitialized by software using the UG bit in the TIMx_EGR register, if URS = 0
    and UDIS = 0 in the TIMx_CR1 register
    */
    TIM6->SR &= ~TIM_SR_UIF;

    /* 翻转LED */
    Driver_LED_Toggle(LED_1);
}
