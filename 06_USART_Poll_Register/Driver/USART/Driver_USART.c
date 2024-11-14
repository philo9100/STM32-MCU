#include "Driver_USART.h"

/**
 * @brief 初始化串口1
 * @param
 */
void Driver_USART1_Init(void)
{
    /* 1.开启串口1时钟 */
    /* 1.1 串口1外设的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* 1.2 GPIO时钟 Port A */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2.配置GPIO引脚的工作模式：PA9 = TX 复用推挽输出（CNF=10 MODE=11）, PA10 = RX 浮空输入（CNF=01 MODE=00）*/
    GPIOA->CRH |= (GPIO_CRH_CNF9_1 & ~GPIO_CRH_CNF9_0 | GPIO_CRH_MODE9);
    GPIOA->CRH |= (~GPIO_CRH_CNF10_1 & GPIO_CRH_CNF10_0 & ~GPIO_CRH_MODE10); // 复位后的默认值就是这个可以不配置

    /* 3.串口参数配置 */
    /* 3.1 配置波特率 115200 根据手册计算 */
    USART1->BRR = 0x271;

    /* 3.2 配置一个字的长度 数据位 8位 在串口的控制寄存器1（Control register 1 (USART_CR1)）中的 M位 配置 */
    USART1->CR1 &= ~USART_CR1_M; // 复位后的默认值就是这个可以不配置

    /* 3.3 配置校验位 无 在串口的控制寄存器1中的 PCE位 配置 */
    USART1->CR1 &= ~USART_CR1_PCE;

    // USART1->CR1 &= ~(USART_CR1_M | USART_CR1_PCE);

    /* 3.4 配置停止位的长度 在串口的控制寄存器2中的 STOP位 配置 */
    USART1->CR2 &= ~USART_CR2_STOP; // 复位后的默认值就是这个可以不配置

    /* 3.5 使能接收和发送 在串口的控制寄存器1中的 TE位 和 RE位 配置 */
    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_RE;

    /* 4.使能串口 在串口的控制寄存器1中的 UE位 配置 */
    USART1->CR1 |= USART_CR1_UE;
}

/**
 * @brief 发送一个字节
 * @param byte
 */
void Driver_USART1_SendChar(uint8_t byte)
{
    /* 1.循环等待发送数据寄存器不为0，否则会一直循环 */
    while (USART1->SR & USART_SR_TXE == 0)
        ;
    /*
    判断串口状态寄存器 USART1->SR 中的表示发送数据寄存器（Transmit Data register (TDR)）的位是否为0，如果不为0，表示有数据要发送
    直到Transmit Data register (TDR)中没有数据要发送为止，再往串口的Transmit Data register (TDR)中写入要发送的数据。
    */

    USART1->DR = byte; // 将要发送的数据写入到Transmit Data register (TDR)中
}

/**
 * @brief 发送一个字符串
 * @param str 要发送的字符串
 * @param len 字符串中字节的长度
 */
void Driver_USART1_SendString(uint8_t *str, uint16_t len)
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
    while (USART1->SR & USART_SR_RXNE == 0)
        ;
    // 判断串口状态寄存器中的表示接收数据寄存器的位（RXNE: Read data register not empty）是否为0，如果为0，表示没有数据接收会一直循环等待
    // 1: Received data is ready to be read.

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
            if (USART1->SR & USART_SR_IDLE != 0) // 判断是否检测到空闲帧，如果检测到空闲帧，表示一个字符串接收完成
            {
                *len = i;
                return;
            }
        }
        buff[i] = USART1->DR; // USART1 的 DR寄存器有九位
        i++;
    }
}

/**
在接收数据时，如果启用了奇偶校验，读取的最高位（第 9 位）将是奇偶校验位。以下是一个具体的例子：

### 示例

假设我们使用 8 位数据长度和奇偶校验功能：

1. **发送数据**：
   - 发送的数据字节为 `0x55`（二进制为 `01010101`）。
   - 假设我们使用奇偶校验来确保数据的奇偶性。`0x55` 中有 4 个 1，因此我们需要设置奇偶校验位为 0（偶数奇偶校验）。

2. **接收数据**：
   - 接收方接收到的数据可能是 `0x55` 和奇偶校验位。假设接收的完整数据为 `0x55`（数据）和 `0`（奇偶校验位）。
   - 在 DR 寄存器中，接收到的值将是：
     - 数据部分：`01010101`（8 位）
     - 奇偶校验位：`0`（第 9 位）

3. **读取数据**：
   - 当接收方读取 DR 寄存器时，读取的值将是 `0x55`，而第 9 位（奇偶校验位）将被存储在最高位。

### 读取示例

```c
uint8_t received_data = USART1->DR & 0xFF; // 读取数据部分
uint8_t parity_bit = (USART1->DR >> 8) & 0x01; // 读取奇偶校验位
``

在这个例子中，`received_data` 将是 `0x55`，而 `parity_bit` 将是 `0`。这样，接收方可以根据奇偶校验位验证接收到的数据的完整性。
*/
