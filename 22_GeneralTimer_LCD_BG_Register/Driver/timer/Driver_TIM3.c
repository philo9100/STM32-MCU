#include "Driver_TIM3.h"

void Driver_TIM3_Init(void)
{
    /* 1.  ����ʱ��*/
    /* 1.1 ��ʱ��3��ʱ�� */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    /* 1.2 GPIO��ʱ�� PA */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2. ����GPIO�ĸ���������� PB0 CNF=10 MODE=11*/
    GPIOB->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0);
    GPIOB->CRL &= ~GPIO_CRL_CNF0_0;

    /* 3. ��ʱ������ */
    /* 3.1 Ԥ��Ƶ�������� */
    TIM3->PSC = 72 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������� */
    TIM3->ARR = 100 - 1;
    /* 3.3 �������ļ������� 0=���� 1=����*/
    TIM3->CR1 &= ~TIM_CR1_DIR;
    /* 3.4 ����ͨ��3�Ĳ���ȽϼĴ��� */
    TIM3->CCR3 = 97;
    /* 3.5 ��ͨ��3����Ϊ���  CCMR1_CC2S=00 */
    TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;
    /* 3.6 ����ͨ��������Ƚ�ģʽ CCMR1_OC2M=110*/
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    /* 3.7 ʹ��ͨ��3  CCER_CC2E=1 */
    TIM3->CCER |= TIM_CCER_CC3E;

    /* 3.8 ����ͨ���ļ��� 0=�ߵ�ƽ��Ч  1=�͵�ƽ��Ч */
    TIM3->CCER &= ~TIM_CCER_CC3P;
}

void Driver_TIM3_Start(void)
{
    /* ʹ�ܼ����� */
    TIM3->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM3_Stop(void)
{
    /* ʧ�ܼ����� */
    TIM3->CR1 &= ~TIM_CR1_CEN;
}

void Driver_TIM3_SetDutyCycle(uint8_t dutyCycle)
{
    TIM3->CCR3 = dutyCycle;
}