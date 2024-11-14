#include "Driver_SysTick.h"
#include "stdio.h"

void Driver_SysTick_Init(void)
{
    /* 1. 配置时钟源  1=AHB(72MHz) 0=AHB/8 */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
    /* 2. 使能中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT;
    /* 3. 定时器 1ms 产生一次中断 */
    SysTick->LOAD = 72000 - 1;
    /* 4. 使能定时器 */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

}

// 每1ms产生一次中断
uint16_t count = 0;
void SysTick_Handler(void)
{
    count++;
    if (count == 1000)
    {
        printf("%d", count);
        count = 0;
        Drviver_LED_Toggle(LED_1);
    }
}