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
 */
void Driver_I2C2_Init(void)
{
    /* PB10:I2C2_SCL  PB11:I2C2_SDA
        时钟线和数据线连接的设备其引脚应该配置成开漏输出模式，原因是：
        1.开漏模式允许多个设备（既可以是主设备，也可以是从设备）共享同一条线路（SCL或者SDA），这是I2C总线的基本要求。
        在开漏模式下，线路（SCL或者SDA）的电平不是由设备直接驱动的，而是通过一个上拉电阻连接到电源，当所有设备都不驱动线路（SCL或者SDA）时，线路（SCL或者SDA）电平为高。任何设备都可以通过将其引脚输出设置为低电平来驱动线路（SCL或者SDA）将电平拉低。

        输出：当设备需要在SDA或SCL上输出低电平时，设备会将对应的引脚设置为低电平。这实际上是将引脚连接到地线，从而将总线上的电平拉低，这样其他设备就可以检测到这个低电平。   
        输入：当设备需要从SDA或SCL读取数据时，它会将对应的引脚设置为高阻态（输入模式）。在这种模式下，设备的引脚不会驱动总线对总线的电平产生影响。因此设备可以通过检测总线电平来读取数据。

        2.开漏模式也允许设备与高于其自身供电电压的设备通信。例如，一个3.3V的设备可以与一个5V的设备通信，只要3.3V的设备不试图驱动线路到高电平。


        配置GPIOB Pin10 11工作模式
        开漏输出：既可以用于输出也可以用于输入，SDA和SCL外界上拉电阻
                用于输入的时候，最好先输出一个1，即设备主动释放对SDA和SCL的控制，也可以不输出，默认就会释放。
        MODE = 11 CNF = 01
    */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //使能GPIOB时钟

    // GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11);
    // GPIOB->CRH |=  ((~GPIO_CRH_CNF10_1 & GPIO_CRH_CNF10_0) | (~GPIO_CRH_CNF11_1 & GPIO_CRH_CNF11_0));
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0);
    GPIOB->CRH &= ~(GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1);
}

/**
 * @brief I2C2启动 起始信号
 * @param  
 * 
 * 参考I2C规范手册的时序图
 */
void Driver_I2C2_Start(void)
{
    /* 1.先拉高SDA和SCL */
    I2C2_SCL_HIGH;
    I2C2_SDA_HIGH;

    /* 2.延时使得SDA和SCL电平状态平稳 */
    I2C2_DELAY_US(10);

    /* 3.拉低SDA，保持SCL为高电平 */
    I2C2_SDA_LOW;

    /* 4.延时使得SDA电平状态平稳 */
    I2C2_DELAY_US(10);
}

/**
 * @brief I2C2停止 停止信号
 * @param  
 * 
 * 参考I2C规范手册的时序图
 */
void Driver_I2C2_Stop(void)
{
    /* 1.拉高SCL，拉低SDA */
    I2C2_SCL_HIGH;
    I2C2_SDA_LOW;

    /* 2.延时使得SDA和SCL电平状态平稳 */
    I2C2_DELAY_US(10);

    /* 3.拉高SDA，保持SCL为高电平 */
    I2C2_SDA_HIGH;

    /* 4.延时使得SDA电平状态平稳 */
    I2C2_DELAY_US(10);
}

/**
 * @brief 接收方产生应答信号 
 * @param  
 * 
 * 参考I2C规范手册的时序图
 * 
 * 主设备负责产生和控制时钟脉冲：在I2C总线上，主设备负责生成SCL时钟信号。主设备通过拉高和拉低SCL线来产生时钟脉冲，从而同步数据传输过程。
 * 从设备（Slave）被动接收时钟信号：从设备不生成时钟信号，它们只是根据主设备提供的SCL时钟来读取或写入数据。
 * 主设备在拉低并延时SCL时，从设备才会向SDA中读取或写入一位数据。
 */
void Driver_I2C2_Ack(void)
{
    /* 1.主设备拉高SDA，主设备拉低SCL，这一步作为应答的开始状态，本质上是主设备发送完数据等待接收方要发送一个应答，和发送方发送一位数据一致 */
    I2C2_SDA_HIGH;
    I2C2_SCL_LOW;

    /* 2.延时使得SDA和SCL电平状态平稳，此时SDA处于高电平稳态，SCL处于低电平稳态，接受方比较完地址后开始发送应答 */
    I2C2_DELAY_US(10);

    /* 3.接受方拉低SDA，保持SCL为低电平 */
    I2C2_SDA_LOW;

    /* 4.延时使得SDA电平状态平稳 */
    I2C2_DELAY_US(10);

    /* 5.主设备拉高SCL，保持SDA为低电平 */
    I2C2_SCL_HIGH;

    /* 6.延时使得SCL电平状态平稳 */
    /* 5和6两步本质上表示接收方应答时发送一个0作为应答信号（数据在SCL高电平期间被采样），SCL处于高电平稳态时，读取到的SDA电平信号为数据信号 */
    I2C2_DELAY_US(10);

    /* 7.主设备拉低SCL，保持SDA为低电平 */
    I2C2_SCL_LOW;

    /* 8.延时使得SCL电平状态平稳 */
    /* 7和8两步本质上表示一位数据发送结束，后面 拉高SDA（产生上升沿）/拉低SDA（产生下降沿）/SDA电平不变 表示发送下一位数据 */
    I2C2_DELAY_US(10);

    /* 9.接收方拉高SDA */
    I2C2_SDA_HIGH;

    /* 10.延时使得SDA电平状态平稳 */
    /* 此时SDA处于高电平稳态（本质上由输出0变为默认的高阻态），表示接收方应答结束，释放对SDA的控制 */
    I2C2_DELAY_US(10);
}

/**
 * @brief 接收方不产生应答信号
 * @param  
 * 
 * 参考I2C规范手册的时序图
 */
void Driver_I2C2_NAck(void)
{
    /* 1.主设备拉高SDA（本质上是主设备释放对SDA的控制），主设备拉低SCL，这一步作为应答的开始状态，本质上是主设备发送完数据等待接收方要发送一个应答，和发送方发送一位数据一致 */
    I2C2_SDA_HIGH;
    I2C2_SCL_LOW;

    /* 2.延时使得SDA和SCL电平状态平稳，此时SDA处于高电平稳态，SCL处于低电平稳态，接受方比较完地址后开始发送应答 */
    I2C2_DELAY_US(10);

    /* 3.主设备拉高SCL，保持SDA为高电平 */
    I2C2_SCL_HIGH;

    /* 4.延时使得SCL电平状态平稳 */
    /* 3和4两步相当于接收方应答时发送一个1作为应答信号（从设备从始至终都没有控制SDA，即没有产生应答信号），SCL处于高电平稳态时，读取到的SDA电平信号为数据信号 */
    I2C2_DELAY_US(10);

    /* 5.主设备拉低SCL，保持SDA为高电平 */
    I2C2_SCL_LOW;

    /* 6.延时使得SCL电平状态平稳 */
    /* 5和6两步相当于接收方一位数据发送结束*/
    I2C2_DELAY_US(10);
}

/**
 * @brief 发送方（主设备）等待接收方（从设备）的应答信号
 * @param  
 * @return 1:接收应答失败 0:接收应答成功 
 */
uint8_t Driver_I2C2_WaitAck(void)
{
    /* 1.主设备拉高SDA（相当于释放对SDA的控制），SDA的高低由从设备来决定，主设备只需要控制SCL同步时钟脉冲即可 */
    I2C2_SDA_HIGH;

    /* 2.主设备拉低SCL（从设备读到拉低的SCL开始发送一个应答，相当于主设备开始同步从设备发送的数据） */
    I2C2_SCL_LOW;

    /* 3.延时使得SCL电平状态平稳，让从设备读到拉低的SCL并开始拉高/拉低/保持不变SDA */
    I2C2_DELAY_US(10);

    /* 4.主设备拉高SCL（数据在SCL高电平期间被采样） */
    I2C2_SCL_HIGH;

    /* 5.延时使得SCL电平状态平稳，并采样此时的SDA的电平状态 */
    I2C2_DELAY_US(10);

    /* 6.读取SDA的电平 */
    // uint8_t ack = ACK;
    // if(I2C2_SDA_READ)
    // {
    //     ack = NACK;
    // }
    uint8_t ack = I2C2_SDA_READ ? NACK : ACK;

    /* 7.主设备拉低SCL，准备下一位的数据发送 */
    I2C2_SCL_LOW;

    /* 8.延时使得SCL电平状态平稳 */
    I2C2_DELAY_US(10);

    return ack;
}

/**
 * @brief I2C2发送一个字节
 * @param byte:要发送的字节
 */
void Driver_I2C2_SendByte(uint8_t byte)
{
    /* 发送一个字节（8位） */
    for (uint8_t i = 0; i < 8; i++)
    {
        /* 1.主设备拉低SCL，同步将要发送的一位数据 */
        I2C2_SCL_LOW;

        /* 2.延时使得SCL电平状态平稳，主设备开始拉高/拉低/保持不变SDA 准备在SDA上发送一位数据，让从设备读到拉低的SCL 开始读取SDA */
        I2C2_DELAY_US(10);

        /* 3.向SDA写一位数据（即拉高/拉低/保持不变 SDA），注意这个写是从高位开始写数据 */
        if (byte & 0x80)
        {
            I2C2_SDA_HIGH;
        } else
        {
            I2C2_SDA_LOW;
        }
        
        I2C2_DELAY_US(10);
        
        /* 4.主设备拉高SCL（数据在SCL高电平期间被采样） */
        I2C2_SCL_HIGH;

        I2C2_DELAY_US(10);

        /* 5.byte左移，发送下一位 */
        byte <<= 1;

    }
}

/**
 * @brief I2C2接收一个字节
 * @param  
 * @return 接收到的字节
 */
uint8_t Driver_I2C2_ReceiveByte(void)
{
    uint8_t data;
    
    for (uint8_t i = 0; i < 8; i++)
    {
        /* 1.主设备拉低SCL */
        I2C2_SCL_LOW;

        /* 2.延时 */
        I2C2_DELAY_US(10);

        /* 3.主设备拉高SCL */
        I2C2_SCL_HIGH;

        /* 4.延时 */
        I2C2_DELAY_US(10);

        /* 6.将读到的高位左移一位，低位补零，如果读下一位是0，恰好是补位的0，如果读下一位是1，则或等上1 ，这里要先左移，否则会把最高位移出去*/
        data <<= 1;

        /* 5.从设备读取SDA */
        if (I2C2_SDA_READ)
        {
            data |= 0x01;
        }

    }
    
    return data;
}













