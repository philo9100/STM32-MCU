#include "Driver_GPIO.h"



void Driver_GPIO_Init()
{
    /* 开启时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;

    /* RST PG7通用推挽 MODE=11 CNF=00 */
    GPIOG->CRL |= (GPIO_CRL_MODE7 & ~GPIO_CRL_CNF7);

    /* INT PG6上拉输入 MODE=00 CNF=10 ODR=1 */
    GPIOG->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6_0);
    GPIOG->CRL |= GPIO_CRL_CNF6_1;
    GPIOG->ODR |= GPIO_ODR_ODR6;
}