#include "Driver_LED.h"

/**
 * @brief  对 LED 灯初始化
 * @param  None
 * @retval None
 */ 
void Driver_LED_Init(void)
{
    /* 1.打开 GPIOA 的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2.给用到的端口的所有引脚（PIN：PA0 PA1 PA8）配置工作模式：通用推挽输出 MODE：11 CNF：00 */
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);    
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);

    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH &= ~GPIO_CRH_CNF8;

    /* 3.关闭所有LED */
    // Driver_LED_OFF(LED_1 | LED_2 | LED_3);
    Driver_LED_OFF(LED_1);
    Driver_LED_OFF(LED_2);
    Driver_LED_OFF(LED_3);
}

/**
 * @brief 点亮指定的 LED 灯 
 * @param led 
 */
void Driver_LED_ON(uint32_t led)
{
    GPIOA->ODR &= ~led;
}

/**
 * @brief 熄灭指定的 LED 灯
 * @param led 
 */
void Driver_LED_OFF(uint32_t led)
{
    GPIOA->ODR |= led;
}

/**
 * @brief 翻转LED状态
 * @param led 
 */
void Driver_LED_Toggle(uint32_t led)
{
    if((GPIOA->IDR & led) == 0)
    {
        Driver_LED_OFF(led);
    }
    else
    {
        Driver_LED_ON(led);
    }
}

/**
 * @brief 打开所有 LED
 * @param leds 
 * @param size 
 */
void Driver_LED_OnAll(uint32_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Driver_LED_ON(leds[i]);
    }
}

/**
 * @brief 关闭所有 LED
 * @param leds 
 * @param size 
 */
void Driver_LED_OffAll(uint32_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Driver_LED_OFF(leds[i]);
    }
}
