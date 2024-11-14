#include "Driver_TIM6.h"
#include "Delay.h"
/**
 * @description: ����ʱ��6���г�ʼ��
 * @return {*}
 */
void Driver_TIM6_Init(void)
{
    /* 1. ����ʱ��6����ʱ��*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    /* 2. ����Ԥ��Ƶֵ: ��Ƶֵ7199��ʾ7200��Ƶ����Ƶ��Ƶ��10K������100us */
    TIM6->PSC = 7200 - 1;

    /*
    3. �����Զ���ת�ؼĴ�����ֵ�������жϷ�����Ƶ�ʡ�
        ��������Ϊ9999����ʾ����������10000�η���һ���жϡ�
        ����һ��100us��10000��1000000us������1s
    */
    TIM6->ARR = 10000 - 1;

    /* Ϊ�˱���һ����������������ж�,�����Ȳ���һ�������¼� */
    TIM6->CR1 |= TIM_CR1_URS;
    TIM6->EGR |= TIM_EGR_UG; /* Ԥ��Ƶ�Ĵ�������װ�ؼĴ�����ֵ���µ�Ӱ�ӼĴ��� */
    // TIM6->SR &= ~TIM_SR_UIF;
    
    /* 4. ʹ�ܸ����ж� */
    TIM6->DIER |= TIM_DIER_UIE;

    /* NVIC���� */
    /* 5. �����ж����ȼ����� */
    NVIC_SetPriorityGrouping(3);
    /* 6. �����ж����ȼ� */
    NVIC_SetPriority(TIM6_IRQn, 1);

    /* 7. ʹ�ܶ�ʱ��6���ж� */
    NVIC_EnableIRQ(TIM6_IRQn);

    /* 8. ʹ�ܼ����� */
    TIM6->CR1 |= TIM_CR1_CEN;
}

/**
 * @description: TIM6���жϷ�����
 * @return {*}
 */
void TIM6_IRQHandler(void)
{
    TIM6->SR &= ~TIM_SR_UIF;
    Drviver_LED_Toggle(LED_1);
    printf("1\r\n");
}
