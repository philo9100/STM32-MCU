#include "Driver_SysTick.h"



void Driver_SysTick_Init(void)
{
    /* 1.配置SysTick Timer 的时钟源 1=AHB(72MHz) 0=AHB/8 */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;

    /* 2.使能中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT;

    /* 3.设置重装载值，SysTick Timer的频率是72MHz，即周期是1/72us
    如果要让定时器周期性地 1ms 产生一次中断，那么就需要配置重装载值为 72000 - 1 
    如果要让定时器只 1ms 产生一次中断，那么就需要配置重装载值为 72000
    */
    SysTick->LOAD = 72000 - 1;
    /* 将SysTick Current Value Register置为0 */
    SysTick->VAL = 0;

    /* 4.使能定时器 */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}



/**
 * @brief SysTick Timer 中断服务函数
 * @param  
 * 
 * 由于 SysTick->CTRL 中 SysTick_CTRL_TICKINT 位置为 1
 * 因此SysTick Timer 每此从重装载值减到0，就会产生一次中断
 */

uint16_t count = 0;

void SysTick_Handler(void){
    count++;
    if (count == 1000 /* 每1ms产生一个中断，count累积到1000，即累计1s */)
    {
        count = 0;
        // printf("已经计时1s\n");
        Driver_LED_Toggle(LED_1);
    }
}





