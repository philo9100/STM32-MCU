#include "Driver_Key.h"
#include "Driver_LED.h"
#include "Delay.h"

/**
 * @description: ��ʼ������.
 *  1. ��������Ӧ��io�����ù���ģʽ: ��������
 *  2. ���ø���Ϊ�ⲿ�ж�
 *  3. �����ⲿ�жϿ����� EXTI
 *  4. ����NVIC
 */
void Driver_Key3_Init(void)
{
    /* 1. ����ʱ�� */
    /* 1.1  GPIOF*/
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    /* 1.2  AFIO*/
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    /* 2. ���� PF10Ϊ��������: MODE=00 CNF=10  ODR=0 */
    GPIOF->CRH &= ~GPIO_CRH_MODE10;
    GPIOF->CRH |= GPIO_CRH_CNF10_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;
    GPIOF->ODR &= ~GPIO_ODR_ODR10;

    /* 3. ����AFIO ����PF10����Ϊ�ⲿ�ж�  EXTICR3  0101*/
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10;
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;

    /* 4. ����EXTI */
    /* 4.1. ���������ش��� RTSR TR10=1*/
    EXTI->RTSR |= EXTI_RTSR_TR10;
    /* 4.2 ���� LINE10, ���õ��ж����μĴ��� */
    EXTI->IMR |= EXTI_IMR_MR10;

    /* 5. ���� NVIC */
    /* 5.1 �������ȼ��� (3-7) ����3��ʾ4��������λȫ�����ڱ�ʾ��ռ���ȼ�*/
    NVIC_SetPriorityGrouping(3);
    /* 5.2 �������ȼ� ����1:�жϺ�*/
    NVIC_SetPriority(EXTI15_10_IRQn, 3);
    /* 5.3 ʹ��Line10 */
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Driver_Key4_Init(void)
{
    /* 1. ����ʱ�� */
    /* 1.1  GPIOF*/
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    /* 1.2  AFIO*/
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    /* 2. ���� PF11Ϊ��������: MODE=00 CNF=10  ODR=0 */
    GPIOF->CRH &= ~GPIO_CRH_MODE11;
    GPIOF->CRH |= GPIO_CRH_CNF11_1;
    GPIOF->CRH &= ~GPIO_CRH_CNF11_0;
    GPIOF->ODR &= ~GPIO_ODR_ODR11;

    /* 3. ����AFIO ����PF11����Ϊ�ⲿ�ж�  EXTICR3  0101*/
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI11;
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI11_PF;

    /* 4. ����EXTI */
    /* 4.1. ���������ش��� RTSR TR11=1*/
    EXTI->RTSR |= EXTI_RTSR_TR11;
    /* 4.2 ���� LINE11, ���õ��ж����μĴ��� */
    EXTI->IMR |= EXTI_IMR_MR11;
}

/**
 * @description: line 15-10���жϷ�����.
 *  һ�������°���3 4,���ִ��һ���������
 * @return {*}
 */
uint8_t isKey3Pressed = 0;
uint8_t isKey4Pressed = 0;
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR10) // Key3����
    {
        /* ���һ������Ҫ����жϱ�־λ */
        EXTI->PR |= EXTI_PR_PR10;

        Delay_ms(5);
        if ((GPIOF->IDR & GPIO_IDR_IDR10) != 0)
        {
            isKey3Pressed = 1;
        }
    }else if(EXTI->PR & EXTI_PR_PR11){ // key4����
        /* ���һ������Ҫ����жϱ�־λ */
        EXTI->PR |= EXTI_PR_PR11;

        Delay_ms(5);
        if ((GPIOF->IDR & GPIO_IDR_IDR11) != 0)
        {
            isKey4Pressed = 1;
        }
    }
}
