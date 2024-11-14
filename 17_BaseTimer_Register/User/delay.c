#include "delay.h"

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
