#include "Driver_LED.h"

/**
 * @description: ��LED���г�ʼ��
 */
void Driver_LED_Init(void)
{
    /* 1. ��GPIOA��ʱ�� */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. ���õ��Ķ˿ڵ����� PIN (PA0 PA1 PA8) ���ù���ģʽ: ͨ��������� MODE:11  CNF:00 */
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);

    GPIOA->CRH |= GPIO_CRH_MODE8;
    GPIOA->CRH &= ~GPIO_CRH_CNF8;

    /* 3. �ر����е�  */
    Drviver_LED_Off(LED_1);
    Drviver_LED_Off(LED_2);
    Drviver_LED_Off(LED_3);
}

/**
 * @description: ����ָ����LED
 * @param {uint32_t} led Ҫ������LED
 */
void Drviver_LED_On(uint32_t led)
{
    GPIOA->ODR &= ~led;
}

/**
 * @description: �ر�ָ����LED
 * @param {uint32_t} led Ҫ�رյ�LED
 */
void Drviver_LED_Off(uint32_t led)
{
    GPIOA->ODR |= led;
}

/**
 * @description: ��תLED��״̬
 * @param {uint32_t} led Ҫ��ת��LED
 */
void Drviver_LED_Toggle(uint32_t led)
{
    /* 1. ��ȡ���ŵĵ�ƽ,�����1(Ŀǰ�ǹر�), ��, ����͹ر� */
    if ((GPIOA->IDR & led) == 0)
    {
        Drviver_LED_Off(led);
    }
    else
    {
        Drviver_LED_On(led);
    }
}

/**
 * @description: �����������еĵ�
 * @param {uint32_t} leds ���е�
 * @param {uint8_t} size �Ƶĸ���
 */
void Drviver_LED_OnAll(uint32_t leds[], uint8_t size)
{

    for (uint8_t i = 0; i < size; i++)
    {
        Drviver_LED_On(leds[i]);
    }
}

/**
 * @description: �ر����������еĵ�
 * @param {uint32_t} leds ���е�
 * @param {uint8_t} size �Ƶĸ���
 */
void Drviver_LED_OffAll(uint32_t leds[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        Drviver_LED_Off(leds[i]);
    }
}
