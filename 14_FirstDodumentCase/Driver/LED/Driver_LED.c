#include "Driver_LED.h"

/**
 * @description: 对LED进行初始化
 */
void Driver_LED_Init(void)
{
    /* 1. 打开GPIOA的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. 给用到的端口的所有 PIN (PA0 PA1 PA8) 设置工作模式: 通用推挽输出 MODE:11  CNF:00 */
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);

    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH &= ~GPIO_CRH_CNF8;

    /* 3. 关闭所有灯  */
    Drviver_LED_Off(LED_1);
    Drviver_LED_Off(LED_2);
    Drviver_LED_Off(LED_3);
}

/**
 * @description: 点亮指定的LED
 * @param {uint32_t} led 要点亮的LED
 */
void Drviver_LED_On(uint32_t led)
{
    GPIOA->ODR &= ~led;
}

/**
 * @description: 关闭指定的LED
 * @param {uint32_t} led 要关闭的LED
 */
void Drviver_LED_Off(uint32_t led)
{
    GPIOA->ODR |= led;
}

/**
 * @description: 翻转LED的状态
 * @param {uint32_t} led 要翻转的LED
 */
void Drviver_LED_Toggle(uint32_t led)
{
    /* 1. 读取引脚的电平,如果是1(目前是关闭), 打开, 否则就关闭 */
    if ((GPIOA->IDR & led) == 1)
    {
        Drviver_LED_Off(led);
    }
    else
    {
        Drviver_LED_On(led);
    }
}

/**
 * @description: 打开数组中所有的灯
 * @param {uint32_t} leds 所有灯
 * @param {uint8_t} size 灯的个数
 */
void Drviver_LED_OnAll(uint32_t leds[], uint8_t size)
{

    for (uint8_t i = 0; i < size; i++)
    {
        Drviver_LED_On(leds[i]);
    }
}

/**
 * @description: 关闭数组中所有的灯
 * @param {uint32_t} leds 所有灯
 * @param {uint8_t} size 灯的个数
 */
void Drviver_LED_OffAll(uint32_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Drviver_LED_Off(leds[i]);
    }
}
