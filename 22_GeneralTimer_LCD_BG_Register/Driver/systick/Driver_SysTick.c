#include "Driver_SysTick.h"
#include "stdio.h"

void Driver_SysTick_Init(void)
{
    /* 1. ����ʱ��Դ  1=AHB(72MHz) 0=AHB/8 */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
    /* 2. ʹ���ж� */
    SysTick->CTRL |= SysTick_CTRL_TICKINT;
    /* 3. ��ʱ�� 1ms ����һ���ж� */
    SysTick->LOAD = 72000 - 1;
    /* 4. ʹ�ܶ�ʱ�� */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

}

// ÿ1ms����һ���ж�
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