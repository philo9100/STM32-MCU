#include "Driver_LED.h"

/**
 * @brief  对LED灯初始化
 * @param  None
 * @retval None
 */
void Driver_LED_Init(void)
{
    /* 1.打开GPIO Port A的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2.给用到的所有引脚（PIN：PA0 PA1 PA8）配置工作模式：通用推挽输出 MODE：11 CNF：00 */
    GPIOA->CRL |= (GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1); // (uint32_t)0x00 00 00 01 | (uint32_t)0x00 00 00 02 配置了PA0和PA1的输出模式
    // GPIOA->CRL |= GPIO_CRL_MODE0;    // 上面的操作可以直接通过宏，将第一位和的第二位直接都配置成1

    GPIOA->CRL &= ~(GPIO_CRL_CNF0_0 | GPIO_CRL_CNF0_1); // ~((uint32_t)0x00 00 00 04 | (uint32_t)0x00 00 00 08) 配置了PA0和PA1的输出模式
    // GPIOA->CRL &= ~(GPIO_CRL_CNF0);  // 上面的操作可以直接通过宏，将第一位和的第二位直接都配置成1，再按位取反得到零

    // 第九个引脚在CRH寄存器中
    GPIOA->CRH |= GPIO_CRH_MODE8; // (uint32_t)0x00000003 配置了PA8的输出模式
    GPIOA->CRH &= ~GPIO_CRH_CNF8; // ~((uint32_t)0x0000000C) 配置了PA8的输出模式

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
 * @brief 通过操作 GPIO_BSRR 寄存器实现
 * @param led
 */
void Driver_LED_ON_security(uint32_t led)
{
    GPIOA->ODR |= led;
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
    GPIOA->ODR |= led; // 使GPIO_ODR_ODRx写入值1 控制PAx引脚输出高电平，熄灭LEDx
}

/**
 * @brief 翻转LED状态
 * @param led 需要翻转的LED
 */
void Driver_LED_Toggle(uint16_t led)
{
    /*
    根据手册可知，在配置输出模式是可以读取引脚的状态，根据读取到的引脚输出值可以知道现在LED是点亮还是熄灭
    通过 IDR 寄存器一次读取到 GPIOA 16 个 Pin 的输出值，而现在只关心控制 LED 的引脚的输出值
    由 ODR 寄存器中用来控制 LED 的位中写入的值来控制 LED 的引脚的输出值
    所以只需要将 IDR 寄存器中16位的值与 ODR 寄存器中只用来控制 LED 引脚输出值的位（第0，1，8位）进行与运算即可
    因为 IDR 寄存器中 16 位的值不确定，所以与 ODR 寄存器中只用来控制 LED 引脚输出值的其中一位（该位为 1，其他位为 0）进行与运算的结果要么是 0，要么是非 0
    如果是 0 ，说明 IDRx 值为 0，LEDx 点亮，否则 LEDx 熄灭
    */
    if ((GPIOA->IDR & led) == 0) // if(!(GPIOA->IDR & led))
    {
        Driver_LED_OFF(led); // 关闭LEDx
    }
    else
    {
        Driver_LED_ON(led); // 打开LEDx
    }
}

/**
 * @brief 打开所有LED
 * @param leds 所有的LED灯
 * @param size 灯的个数
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
 * @param leds 所有的LED灯
 * @param size 灯的个数
 */
void Driver_LED_OffAll(uint16_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Driver_LED_OFF(leds[i]);
    }
}
