#include "Driver_I2C2.h"

/* 在 .h 中改成宏 */
void I2C2_Delay(uint16_t us)
{
    Delay_us(us);
}

/**
 * @brief I2C2初始化
 * @param
 *
 * 通过寄存器硬件控制I2C
 */
void Driver_I2C2_Init(void)
{
    /* 1.开启时钟 */
    /* 1.1 开启I2C外设硬件的时钟，I2C2外设挂在APB1总线上 */
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    /* 1.2 开启I2C GPIO Pin的时钟，STM32F103芯片作为I2C2的主设备，通过GPIO Pin来控制I2C的SDA和SCL线 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // 使能GPIOB时钟

    /* 2.设置I2C GPIO Pin的工作模式 */
    /* PB10:I2C2_SCL  PB11:I2C2_SDA
        时钟线和数据线连接的设备其引脚应该配置成开漏输出模式，原因是：
        1.开漏模式允许多个设备（既可以是主设备，也可以是从设备）共享同一条线路（SCL或者SDA），这是I2C总线的基本要求。
        在开漏模式下，线路（SCL或者SDA）的电平不是由设备直接驱动的，而是通过一个上拉电阻连接到电源，当所有设备都不驱动线路（SCL或者SDA）时，线路（SCL或者SDA）电平为高。
        任何设备都可以通过将其引脚输出设置为低电平来驱动线路（SCL或者SDA）将电平拉低。

        输出：当设备需要在SDA或SCL上输出低电平时，设备会将对应的引脚设置为低电平。这实际上是将引脚连接到地线，从而将总线上的电平拉低，这样其他设备就可以检测到这个低电平。
        输入：当设备需要从SDA或SCL读取数据时，它会将对应的引脚设置为高阻态（输入模式）。在这种模式下，设备的引脚不会驱动总线对总线的电平产生影响。因此设备可以通过检测总线电平来读取数据。

        2.开漏模式也允许设备与高于其自身供电电压的设备通信。例如，一个3.3V的设备可以与一个5V的设备通信，只要3.3V的设备不试图驱动线路到高电平。


        配置GPIOB Pin10 11工作模式
        通过寄存器硬件控制I2C 默认的Pin不是用来控制I2C的，通过复用GPIO Pin来控制I2C
        复用开漏输出：既可以用于输出也可以用于输入，SDA和SCL外接上拉电阻默认是高电平
            用于输入的时候，最好先输出一个1，即拉高SDA和SCL，即设备主动释放对SDA和SCL的控制。
            也可以不输出，因为设备释放对SDA和SCL线的控制是会改为输入模式（高阻态），外接的上拉电阻会拉高SDA和SCL的电平。
        复用开漏输出，配置如下：
        MODE = 11 CNF = 11
    */
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11);


    /* 3.设置I2C2的寄存器，通过配置寄存器来配置I2C的参数 */
    /* 3.1 禁用I2C外设：在配置I2C参数之前，首先需要禁用I2C外设，以便安全地修改寄存器。*/
    /*
    如果在通信进行时重置该位，则当返回到空闲状态时，即在当前通信结束时禁用I2C外设。
    由于PE置为0，所有位重置将在通信结束时发生。
    注意：在主模式下，该位在通信结束之前不得复位。
    */
    I2C2->CR1 &= ~I2C_CR1_PE;

    /* 配置I2C 为I2C mode 而不是SMBus mode */
    I2C2->CR1 &= ~I2C_CR1_SMBUS;

    /* 3.2 设置I2C时钟频率：配置I2C时钟频率，通常通过设置I2C_CR2寄存器中的FREQ字段来实现。该字段的值应根据系统时钟频率进行计算。*/
    I2C2->CR2 |= 0b100100 << 0; // I2C2挂在APB1总线上配置为36MHz

    /* 3.3 配置时钟控制寄存器（CCR）：设置I2C_CCR寄存器以定义I2C在控制SCL时的时钟控制。根据所需的I2C速率（如标准模式100kHz或快速模式400kHz）计算CCR值。*/
    /* 配置I2C的时钟控制模式，这里配置为标准模式100kHz 对应的时钟周期是10us 按照一半的占空比，则THigh=Tlow=5us */
    I2C2->CCR &= ~I2C_CCR_FS;
    /* 配置I2C产生时钟的频率
        Thigh = CCR * Tclk1
        CCR = THigh / Tclk1 = 5us / (1/36)us = 180
    */
    I2C1->CCR |= 0b10110100 << 0; // 左移0位，刚好是从最低位开始写

    /* 3.4 设置上升沿时间（TRISE）：配置I2C_TRISE寄存器以设置I2C总线的上升时间。该值通常根据I2C速率和系统时钟进行计算。*/
    /* 标准模式下最大上升时间为1000 ns（1微秒）
        TRISE的计算公式：
            TRISE = (最大允许上升时间/Tclk1) + 1
                  = (1/1/36) + 1 = 37
     */
    I2C2->TRISE &= 0b100101 << 0; // 左移0位，刚好是从最低位开始写

    /* 3.5 选择I2C模式：配置I2C_OAR1寄存器以设置I2C的地址模式（7位或10位），并设置设备地址。*/
    // I2C2->OAR1 =

    /* 3.6 使能I2C外设：在所有配置完成后，重新使能I2C外设。 */
    I2C2->CR1 |= I2C_CR1_PE;
}

/**
 * @brief I2C2启动 起始信号
 * @param
 *
 */
uint8_t Driver_I2C2_Start(void)
{
    /* 主设备产生一个起始信号 */
    I2C2->CR1 |= I2C_CR1_START;

    /* 设置一个超时时间， */
    uint16_t timeout = 0xFFFF;

    /*  判断是否产生了一个起始信号
    SB: Start bit (Master mode)
    0: No Start condition
    1: Start condition generated.
    – Set when a Start condition generated.
    – Cleared by software by reading the SR1 register followed by writing the DR register, or by
    hardware when PE=0
    */
    while (((I2C2->SR1 & I2C_SR1_SB) == 0) && timeout)
    {
        timeout--;
    }

    return timeout ? OK : FAIL;
}

/**
 * @brief I2C2停止 停止信号
 * @param
 *
 */
void Driver_I2C2_Stop(void)
{
    /* 主设备产生一个终止信号 */
    I2C2->CR1 |= I2C_CR1_STOP;
}

/**
 * @brief 接收方产生应答信号
 * @param
 */
void Driver_I2C2_Ack(void)
{
    /* 从设备产生一个应答信号 */
    I2C2->CR1 |= I2C_CR1_ACK;
}

/**
 * @brief 接收方不产生应答信号
 * @param
 *
 */
void Driver_I2C2_NAck(void)
{
    /* 从设备产生一个非应答信号 */
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

/**
 * @brief I2C2发送一个字节
 * @param byte:要发送的字节
 */
uint8_t Driver_I2C2_SendByte(uint8_t byte)
{
    /* 超时时间 */
    uint16_t timeout = 0xFFFF;
    /*
    TxE: Data register empty (transmitters)
    0: Data register not empty
    1: Data register empty
    – Set when DR is empty in transmission. TxE is not set during address phase.
    – Cleared by software writing to the DR register or by hardware after a start or a stop condition
    or when PE=0.
    TxE is not set if either a NACK is received, or if next byte to be transmitted is PEC (PEC=1)
    Note: TxE is not cleared by writing the first data being transmitted, or by writing data when
    BTF is set, as in both cases the data register is still empty.
    */
    while (((I2C2->SR1 & I2C_SR1_TXE) == 0) && timeout)
    {
        timeout--;
    }

    /* 将要发送的是数据写入到数据寄存器 */
    timeout ? I2C2->DR = byte : Driver_I2C2_Stop();

    timeout = 0xFFFF;

    /*
    BTF: Byte transfer finished
    0: Data byte transfer not done
    1: Data byte transfer succeeded
    – Set by hardware when NOSTRETCH=0 and:
    – In reception when a new byte is received (including ACK pulse) and DR has not been read
    yet (RxNE=1).
    – In transmission when a new byte should be sent and DR has not been written yet (TxE=1).
    – Cleared by software reading SR1 followed by either a read or write in the DR register or by
    hardware after a start or a stop condition in transmission or when PE=0.
    Note: The BTF bit is not set after a NACK reception
    The BTF bit is not set if next byte to be transmitted is the PEC (TRA=1 in I2C_SR2
    register and PEC=1 in I2C_CR1 register)
    */
    while (((I2C2->SR1 & I2C_SR1_BTF) == 0) && timeout)
    {
        timeout--;
    }

    return timeout ? OK : FAIL;
}

/**
 * @brief I2C2发送一个设备地址
 * @param addr:要发送的地址
 */
uint8_t Driver_I2C2_SendAddr(uint8_t addr)
{
    /* 超时时间 */
    uint16_t timeout = 0xFFFF;
    /*
    TxE: Data register empty (transmitters)
    0: Data register not empty
    1: Data register empty
    – Set when DR is empty in transmission. TxE is not set during address phase.
    – Cleared by software writing to the DR register or by hardware after a start or a stop condition
    or when PE=0.
    TxE is not set if either a NACK is received, or if next byte to be transmitted is PEC (PEC=1)
    Note: TxE is not cleared by writing the first data being transmitted, or by writing data when
    BTF is set, as in both cases the data register is still empty.
    */
    while (((I2C2->SR1 & I2C_SR1_TXE) == 0) && timeout)
    {
        timeout--;
    }

    /* 将要发送的是数据写入到数据寄存器 */
    timeout ? I2C2->DR = addr : Driver_I2C2_Stop();

    timeout = 0xFFFF;

    /*
    ADDR: Address sent (master mode)/matched (slave mode)
    This bit is cleared by software reading SR1 register followed reading SR2, or by hardware
    when PE=0.
    Address matched (Slave)
    0: Address mismatched or not received.
    1: Received address matched.
    – Set by hardware as soon as the received slave address matched with the OAR registers
    content or a general call or a SMBus Device Default Address or SMBus Host or SMBus Alert
    is recognized. (when enabled depending on configuration).
    Note: In slave mode, it is recommended to perform the complete clearing sequence (READ
    SR1 then READ SR2) after ADDR is set. Refer to Figure 272.
    Address sent (Master)
    0: No end of address transmission
    1: End of address transmission
    – For 10-bit addressing, the bit is set after the ACK of the 2nd byte.
    – For 7-bit addressing, the bit is set after the ACK of the byte.
    Note: ADDR is not set after a NACK reception
    */
    while (((I2C2->SR1 & I2C_SR1_ADDR) == 0) && timeout)
    {
        timeout--;
    }

    /* 手册中说明这里需要读一下SR2 来清除之前读的地址 否则之后无法写入地址 */
    if (timeout)
    {
        I2C2->SR2;
    }

    return timeout ? OK : FAIL;
}

/**
 * @brief I2C2接收一个字节
 * @param
 * @return 接收到的字节
 */
uint8_t Driver_I2C2_ReceiveByte(void)
{
    uint16_t timeout = 0xffff;

    while (((I2C2->SR1 & I2C_SR1_RXNE) == 0) && timeout)
    {
        timeout--;
    }

    // 把数据寄存器的值返回
    return timeout ? I2C2->DR : 0;
}
