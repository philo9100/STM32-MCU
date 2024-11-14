#include "Driver_USART.h"

uint8_t buff[100] = {0};  // 接收字符数据缓冲区
uint8_t len = 0;    // 接收字符数据长度

uint8_t isSend = 0; // 是否发送数据标志位


/**
 * @brief 初始化串口1
 * @param  
 */
void Driver_USART1_Init (void) {

    /* 1.开启串口1时钟 */
    /* 1.1 串口1外设的时钟*/
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    
    /* 1.2 GPIO时钟 Port A */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2.配置GPIO引脚的工作模式：PA9 = TX 复用推挽输出（CNF=10 MODE=11）, PA10 = RX 浮空输入（CNF=01 MODE=00）*/
    GPIOA->CRH |= GPIO_CRH_CNF9_1 & ~GPIO_CRH_CNF9_0 | GPIO_CRH_MODE9;
    GPIOA->CRH |= ~GPIO_CRH_CNF10_1 & GPIO_CRH_CNF10_0 & ~GPIO_CRH_MODE10; // 复位后的默认值就是这个可以不配置

    /* 3.串口参数配置 */
    /* 3.1 配置波特率 115200 根据手册计算 */
    USART1->BRR = 0x271;

    /* 3.2 配置一个字的长度 数据位 8位 在串口的控制寄存器1中的 M位 配置 */
    USART1->CR1 &= ~USART_CR1_M; // 复位后的默认值就是这个可以不配置

    /* 3.3 配置校验位 无 在串口的控制寄存器1中的 PCE位 配置 */
    USART1->CR1 &= ~USART_CR1_PCE;

    /* 3.4 配置停止位的长度 在串口的控制寄存器2中的 STOP位 配置 */
    USART1->CR2 &= ~USART_CR2_STOP; // 复位后的默认值就是这个可以不配置

    /* 3.5 使能接收和发送 在串口的控制寄存器1中的 TE位 和 RE位 配置 */
    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_RE;

    /* 3.6 使能串口的各种中断*/
    USART1->CR1 |= USART_CR1_RXNEIE; // 使能接收缓冲区非空中断
    USART1->CR1 |= USART_CR1_IDLEIE; // 使能空闲中断

    /* 4 配置NVIC */
    /* 4.1 配置优先级组 */
    NVIC_SetPriorityGrouping(3);
    /* 4.2 设置优先级 */
    NVIC_SetPriority(USART1_IRQn, 2);
    /* 4.3 使能串口1的中断 */
    NVIC_EnableIRQ(USART1_IRQn);

    /* 5.使能串口 在串口的控制寄存器1中的 UE位 配置 */
    USART1->CR1 |= USART_CR1_UE;
}


/**
 * @brief 发送一个字节
 * @param byte 
 */
void Driver_USART1_SendChar(uint8_t byte)
{
    /* 1.循环等待发送数据寄存器不为0，否则会一直循环 */
    while(USART1->SR & USART_SR_TXE == 0);  // 判断串口状态寄存器中的表示发送数据寄存器的位是否为0，如果不为0，表示有数据要发送
        
    USART1->DR = byte;  // 将要发送的数据写入到数据寄存器中
}

/**
 * @brief 要发送的字符串
 * @param str 要发送的字符串
 * @param len 字符串长度
 */
void Driver_USART1_SendString(uint8_t *str, uint32_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        Driver_USART1_SendChar(str[i]);
    }
}

/**
 * @brief 接收一个字节的数据
 * @param  
 * @return 接收到的字节
 */
uint8_t Driver_USART1_ReceiveByte(void)
{
    uint16_t i = 0xffff;    // 超时时间
    while (USART1->SR & USART_SR_RXNE == 0 && i--);  // 判断串口状态寄存器中的表示接收数据寄存器的位是否为0，如果为0，表示没有数据接收会一直循环等待，如果超时了就退出循环

    return USART1->DR;
}

/*
 * @brief 接收变长数据，接收到的数据存入的buff中，接收到的字节数存入len中
 * @param buff 接收缓冲区
 * @param len 接收长度
 * 
 */
void Driver_USART1_ReceiveString(uint8_t *buff, uint8_t *len)
{
    uint8_t i = 0;
    while (1)
    {
        while (USART1->SR & USART_SR_RXNE == 0) // 判断串口状态寄存器中的表示接收数据寄存器的位是否为0，如果为0，表示没有数据接收
        {
            if(USART1->SR & USART_SR_IDLE != 0) // 判断是否检测到空闲帧，如果检测到空闲帧，表示一个字符串接收完成
            {
                *len = i;
                return;
            }
        }
        buff[i] = USART1->DR;
        i++;
    }
}


/**
 * @brief 串口1中断处理函数，一旦接收到数据或者检测到空闲帧，就会触发中断，不用再轮询等待
 * @param  
 */
void USART1_IRQHandler(void)
{
    if(USART1->SR & USART_SR_RXNE != 0) // 判断是否是接收到数据产生的中断
    {
        /* 对USART_DR的读操作可以将USART_SR的RXNE位清空，否则就要手动清除，这里对DR寄存器进行读操作，因此不想需要手动清除 */
        // USART1->SR &= ~USART_SR_RXNE; // 清除接收数据寄存器非空标志位
        buff[len] = USART1->DR;
        len++;
    }

    if (USART1->SR & USART_SR_IDLE != 0) // 判断是否检测到空闲帧产生的中断
    {
        /* 清除USART_SR的IDLE位需要 先读USART_SR 再读USART_DR */
        USART1->SR; // 读取SR寄存器
        USART1->DR; // 读取DR寄存器
        
        /* 变长数据接收完毕，可以处理一下需要的业务，这里回显一下 */
        // Driver_USART1_SendString(buff, len); // 不要在中断处理函数出执行耗时的操作，避免低优先级的中断无法及时执行
        // len = 0;    // 接收长度清零
        
        /* 使用标志位的方式处理，一旦收到中断就改变标志位，在外面轮询改变的标志位执行发送操作 */
        isSend = 1;        
       
    }
    
}

/**
 * @brief 重定向c库函数printf到串口1 当调用printf函数时，会自动调用这个函数
 * @param ch 字符
 * @param file 文件指针
 * @return 字符
 */
int fputc(int c, FILE *file)
{
    Driver_USART1_SendChar(c);
    return c;
}



