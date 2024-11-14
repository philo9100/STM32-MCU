#include "Driver_KEY.h"

/**
 * @brief 初始化按键
 * @param  
 * 
 * 1.给按键对应的GPIO口 Pin 设置工作模式：根据原理图配置为下拉输入模式
 * 2.配置引脚复用选择器 AFIO    将GPIO Port下的 Pin 复用为AFIO下的外部中断控制器 EXTI 下的 Line
 * 3.配置外部中断控制器 EXTI    
 * 4.配置嵌套向量中断控制器 NVIC
 */
void Driver_KEY_Init(void)
{
    /* 1.开启时钟 */
    /* 1.1 使能GPIOF时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
    // 开启GPIOF时钟时会警告未识别标识符，原因是没有配置对应芯片的型号，默认芯片的型号不支持GPIOF
    // 解决方案是在stm32f10x.h文件中开启对应芯片的型号
    
    /* 1.2 使能AFIO时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    /* 2.配置PF10 为下拉输入模式：MODE=00 CNF=10 配置为上拉/下拉 ODR=0 配置为下拉 */   
    GPIOF->CRH &= ~GPIO_CRH_MODE10;     // (uint32_t)0x00000300
    
    GPIOF->CRH &= ~GPIO_CRH_CNF10_0;    // (uint32_t)0x00000400
    GPIOF->CRH |= GPIO_CRH_CNF10_1;     // (uint32_t)0x00000800
    // GPIOF->CRH = (GPIOF->CRH & ~GPIO_CRH_CNF10_0) | GPIO_CRH_CNF10_1; // 上面的两步可以改写为一步

    GPIOF->ODR &= ~GPIO_ODR_ODR10;

    /* 3.配置引脚复用选择器 AFIO 配置PF10引脚为外部中断LINE 在AFIO的EXTICR3中配置 0101
       根据参考手册，将 GPIO 复用为 AFIO 下的外部中断控制寄存器 EXTICRx，由于 GPIOx（A..G）下有相同编号的16个Pin
       而同一时刻不同Port下相同编号的Pin只能有一个配置为外部中断LINE，如果配置 Port F 下的 Pin 10 为外部中断，则其他 Port 下的 Pin10 不能再配置为外部中断
       在AFIO Register中找到EXternal interrupt configuration register(AFIO_EXTICRx) 通过手册知道 AFIO_EXTICR3 寄存器用来配置 Pin10
       由于Pin10（EXTI10）在AFIO_EXTICR3的位置，所以要配置AFIO_EXTICR3寄存器，将 EXTI10[3:0] 配置为 0101 表示是 Port F 下的 Pin10 
    */
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10; // 清零
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;

    /* 4.配置EXTI 
        在EXTI Register中找到Rising trigger selection register (EXTI_RTSR)，配置上升沿触发中断
        在EXTI Register中找到Interrupt mask register (EXTI_IMR)，不屏蔽LINE10
    */
    /* 4.1 配置上升沿触发 EXTI RTSR TR10 = 1 */
    EXTI->RTSR |= EXTI_RTSR_TR10;
       
    /* 4.2 开启LINE10，配置中断屏蔽寄存器 Interrupt mask register (EXTI_IMR)
    MRx: 线x上的中断屏蔽 (Interrupt Mask on line x) 
    0：屏蔽来自线x上的中断请求；
    1：开放来自线x上的中断请求。
    */
    EXTI->IMR |= EXTI_IMR_MR10;

    /* 5.配置NVIC 
        NVIC是ARM Cortex-M3内核的一部分，用于管理中断。需要使用ARM提供的接口函数来配置NVIC 
        CMSIS是ARM提供的接口函数库，其中core_cm3.h中的函数可以用来配置NVIC
    */
    /* 5.1 设置优先级组 （3-7）配置3表示4个二进制位全部用于表示抢占优先级（组优先级）*/
    /* 见手册 
    extension://oikmahiipjniocckomdccmplodldodja/pdf-viewer/web/viewer.html?file=https%3A%2F%2Fwww.st.com%2Fresource%2Fen%2Fprogramming_manual%2Fcd00228163-stm32f10xxx-20xxx-21xxx-l1xxxx-cortex-m3-programming-manual-stmicroelectronics.pdf#%5B%7B%22num%22%3A365%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C124%2C685%2Cnull%5D
    */
    NVIC_SetPriorityGrouping(3);
    /* 5.2 配置中断优先级 */
    NVIC_SetPriority(EXTI15_10_IRQn, 3);
    /* 5.3 使能LINE10 */
    NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/**
 * @brief 中断服务函数 获取按键状态
 * 芯片厂商会定义好中断服务函数的接口，可以在startup_stm32f10x.s文件中找到对应的中断服务函数名
 * 然后在自己的工程中实现这个接口，这样就可以在对应中断发生时自动调用这个函数，按键按下后会触发对应的中断，执行这个中断服务函数
 * @param  
 * @return 按键状态
 */
void EXTI15_10_IRQHandler(void)
{
    /* 
    EXTI 下的 Pending register (EXTI_PR) 中由 AFIO_EXTICR3_EXTI10 (Port F Pin10) 产生的中断将对应的中断挂起标志位（PR10）置为1
    在执行到这个中断服务函数中后，表示该中断正在被处理，因此一定要将该标志位置为0，否则该中断标志位在请求挂起寄存器中始终处于挂起状态（该位置为1）
    在该中断由NVIC执行一次后NVIC发现请求挂起寄存器中该中断还在挂起会再次执行该中断，这样会一直执行下去
    */
    EXTI->PR |= EXTI_PR_PR10;


    Delay_ms(15); // 消抖
    /* 中断函数中是不建议使用延时函数的
    1.中断服务函数应尽可能快地执行完毕，以便系统可以返回到被中断的任务或处理其他中断。如果在中断服务函数中使用延时函数，会导致中断服务函数执行时间过长，可能会影响系统的实时性和性能。

    2.延时函数通常依赖于系统的定时器，而这些定时器在中断服务程序中可能无法正常工作。例如，如果延时函数依赖于系统的滴答定时器，而滴答定时器的中断优先级低于当前的中断，那么在当前中断服务程序中，滴答定时器的中断将无法得到处理，导致延时函数无法正常工作。

    3.如果在中断服务函数中使用了延时函数，可能会导致嵌套中断的复杂性增加。例如，如果在一个高优先级的中断服务程序中使用了延时函数，而在延时期间发生了一个低优先级的中断，那么低优先级的中断将被延迟处理，可能会导致系统的实时性降低。   
    */
    if (GPIOF->IDR & GPIO_IDR_IDR10 /* 说明此时按键处于按下状态，而非抖动 */) 
    {   
        Driver_LED_Toggle(LED_1);
    }

}



