#include "Driver_LED.h"

/**
 * @brief  对LED灯初始化
 * @param  None
 * @retval None
 */ 
void Driver_LED_Init(void)
{
    /* 1.打开GPIOA的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2.给用到的所有引脚（PIN：PA0 PA1 PA8）配置工作模式：通用推挽输出 MODE：11 CNF：00 */
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);    // (uint32_t)0x00000003 | (uint32_t)0x00000030 配置了PA0和PA1的输出模式
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);     // ~((uint32_t)0x0000000C | (uint32_t)0x000000C0) 配置了PA0和PA1的输出模式

    // 第九个引脚在CRH寄存器中
    GPIOA->CRH |= GPIO_CRH_MODE8;   // (uint32_t)0x00000003 配置了PA8的输出模式 
    GPIOA->CRH &= ~GPIO_CRH_CNF8;   // ~((uint32_t)0x0000000C) 配置了PA8的输出模式
    
    /* 3.关闭所有LED */
    GPIOA->BSRR = GPIO_BSRR_BS0 | GPIO_BSRR_BS1 | GPIO_BSRR_BS8; // 使GPIO_ODR_ODR0、GPIO_ODR_ODR1和GPIO_ODR_ODR8写入值1 分别控制PA0、PA1和PA8引脚输出高电平
}

/**
 * @brief 点亮指定的LED灯
 * 
 * 可以直接通过宏实现
 *  
 * @param led 根据定义的宏，选择LED1、LED2或LED3分别是ODR对应的位的值 类型是uint16_t
 */
void Driver_LED_ON(uint16_t led)
{
    GPIOA->ODR &= ~led; // 使GPIO_ODR_ODRx写入值0 控制PAx引脚输出低电平，点亮LEDx
}

/**
 * @brief 熄灭指定的LED灯
 * 
 * 可以直接通过宏实现
 * 
 * @param led 根据定义的宏，选择LED1、LED2或LED3分别是ODR对应的位的值 类型是uint16_t 
 */
void Driver_LED_OFF(uint16_t led)
{
    GPIOA->ODR |= led;  // 使GPIO_ODR_ODRx写入值1 控制PAx引脚输出高电平，熄灭LEDx  
}

/**
 * @brief 翻转LED状态
 * @param led 
 */
void Driver_LED_Toggle(uint16_t led)
{
    // 读取A组输入数据寄存器的值，一次读取A组中所有引脚的状态与A组输出数据寄存器指定位控制的引脚LEDx做与运算，定义的宏LEDx--ODRx值为1
    // 如果结果为0，说明IDRx值为0，LEDx点亮，否则LEDx熄灭
    if((GPIOA->IDR & led) == 0)
    {
        Driver_LED_OFF(led);    // 关闭LEDx
    }
    else
    {
        Driver_LED_ON(led);     // 打开LEDx
    }
}

/**
 * @brief 打开所有LED
 * @param leds 
 * @param size 
 */
void Driver_LED_OnAll(uint16_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Driver_LED_ON(leds[i]);
    }
}

/**
 * @brief 关闭所有LED
 * @param leds 
 * @param size 
 */
void Driver_LED_OffAll(uint16_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Driver_LED_OFF(leds[i]);
    }
}
