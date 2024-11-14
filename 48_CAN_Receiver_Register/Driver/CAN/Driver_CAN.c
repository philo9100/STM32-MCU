#include "Driver_CAN.h"

/**
 * @brief 初始化 CAN通讯
 * @param
 */
void Driver_CAN_Init(void)
{
    /* 1.开启时钟 CAN时钟 和 GPIO时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;                        // 开启CAN总线的时钟
    RCC->APB2ENR |= (RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN); // 开启GPIO的时钟

    /* 2.给引脚 CANRX 和 CANTX 配置工作模式 */
    /*
    芯片的CAN外设CANRX和CANTX默认使用的是PA11和PA12引脚，现在的需求是使用PB8和PB9作为CANRX和CANTX的引脚
    因此要使用AFIO的重映射功能，找到AFIO的 AF remap and debug I/O configuration register (AFIO_MAPR)
    其中 Bits 14:13 CAN_REMAP[1:0]: CAN alternate function remapping 用于配置CAN的重映射功能
    CAN 备用功能重新映射 这些位由软件设置和清除。
    它们控制具有单个 CAN 接口的设备中 CAN_RX 和 CAN_TX 的替代功能的映射。
    00：CAN_RX映射到 PA11，CAN_TX映射到 PA12（默认）
    01：保留
    10：CAN_RX映射到 PB8，CAN_TX映射到 PB9（在 36 引脚封装上不可用）
    11：CAN_RX映射到 PD0，CAN_TX映射到 PD1
    */
    /* 2.1 将CAN_RX映射到 PB8，CAN_TX映射到 PB9 这里我们配置10*/
    AFIO->MAPR |= (AFIO_MAPR_CAN_REMAP_1 & ~AFIO_MAPR_CAN_REMAP_0);

    /* 2.2 初始化GPIOPB
    PB8 浮空输入：CNF=01 MODE=00
    PB9 复用推挽输出：CNF=10 MODE=11
    */
    GPIOB->CRH |= (~GPIO_CRH_CNF8_1 & GPIO_CRH_CNF8_1 & ~GPIO_CRH_MODE9);
    GPIOB->CRH |= (GPIO_CRH_CNF9_1 & ~GPIO_CRH_CNF9_0 | GPIO_CRH_MODE9);

    /* 3.初始化 CAN */
    /* 3.1 对CAN进行复位 CAN会进入睡眠模式 */
    CAN1->MCR |= CAN_MCR_RESET;

    /* 3.2 退出睡眠模式
    进入睡眠模式：

    软件设置SLEEP位，以请求CAN硬件进入睡眠模式。
    当当前的CAN活动（如数据传输或接收）完成后，硬件将进入睡眠模式。

    退出睡眠模式：

    软件清除SLEEP位，以使CAN硬件退出睡眠模式。

    硬件行为：

    当AWUM（自动唤醒模式）位被设置，并且在CAN接收信号上检测到SOF（起始帧）位时，硬件会清除SLEEP位，从而退出睡眠模式。

    重置状态：

    在复位后，SLEEP位会被设置，表示CAN控制器默认在睡眠模式下启动。
    */
    CAN1->MCR &= ~CAN_MCR_SLEEP;

    /* 等待退出睡眠模式，退出睡眠模式后该位由硬件置为0
    休眠确认位（Sleep Acknowledge）：

    该位由硬件设置，表示CAN硬件已经进入休眠模式。
    这是对软件请求进入休眠模式的确认，软件通过在CAN_MCR寄存器中设置SLEEP位来请求。

    离开休眠模式：

    当CAN硬件离开休眠模式时，该位会被硬件清除。
    离开休眠模式的条件是监测到CAN RX信号上连续的11个隐性信号（recessive bits），以确保硬件与CAN总线的同步。

    清除SLEEP位：

    清除SLEEP位的操作触发了离开休眠模式的过程。
    */
    while (CAN1->MSR & CAN_MSR_SLAK)
        ;

    /* 3.3 进入初始化模式
    进入正常模式：

    软件清除INRQ位，要求CAN硬件切换到正常工作模式。
    当接收信号（Rx）上检测到11个连续的显性位时，CAN硬件就会同步，准备进行数据传输和接收。
    硬件通过清除INAK位来表示已准备好进入正常模式。

    进入初始化模式：

    软件设置INRQ位，请求CAN硬件进入初始化模式。
    一旦设置，该硬件会等待当前的数据传输或接收完成后，再进入初始化模式。
    进入初始化模式后，硬件会通过设置INAK位来指示这一状态。
    */
    CAN1->MCR |= CAN_MCR_INRQ;

    /* 等待进入初始化模式，进出初始化模式后硬件会通过设置INAK位来指示这一状态
    初始化模式指示：

    当硬件设置INAK位时，表示CAN硬件已成功进入初始化模式。这是对软件设置的INRQ位（初始化请求）的确认。

    离开初始化模式：

    当CAN硬件退出初始化模式时，硬件会清除INAK位。这意味着CAN硬件已经准备好在CAN总线上进行同步。
    为了达到同步状态，硬件必须在CAN RX信号上监测到11个连续的“非显性位”（recessive bits）。这是一种确保网络状态稳定的机制。
    */
    while (!(CAN1->MSR & CAN_MSR_INAK))
        ;

    /* 3.4 配置CAN */
    /* 3.4.1 配置自动离线管理 允许自动退出离线状态  */
    CAN1->MCR |= CAN_MCR_ABOM;

    /* 3.4.2 自动唤醒 检测到有报文时，可以从睡眠模式由硬件自动唤醒 */
    CAN1->MCR |= CAN_MCR_AWUM;

    /* 3.4.3 NART：无自动重传
    0：CAN 硬件将自动重传报文，直到根据 CAN 标准成功传输。
    1：一条消息将只传输一次，与传输结果无关（成功、错误或仲裁失败）
    */
    CAN1->MCR &= ~CAN_MCR_NART;

    /* 3.4.4 TXFP：Transmit FIFO priority
    此位控制多个发送邮箱同时挂起时的传输顺序。
    0：由消息标识符驱动的优先级
    1：由请求顺序驱动的优先级（按时间顺序） */
    CAN1->MCR |= CAN_MCR_TXFP;

    /* 3.4.5 RFLM： Receive FIFO locked mode
    0：接收 FIFO 在溢出时未锁定。一旦接收 FIFO 已满，下一条传入消息将覆盖上一条消息。
    1：接收 FIFO 锁定，以防止溢出。一旦接收 FIFO 已满，下一条传入消息将被丢弃
    */
    CAN1->MCR &= ~CAN_MCR_RFLM;

    /* 3.4.6
    0：禁用时间触发通信模式
    当TTCM位为0时，CAN通信将使用传统的事件驱动通信模式。数据的发送和接收基于CAN总线上消息的产生和接收。

    1：启用时间触发通信模式
    当TTCM位为1时，CAN硬件将进入时间触发通信模式。在此模式下，消息的发送和接收被安排在特定的时间窗口内，而不是仅依赖于事件的发生。
    */

    /* 3.5 配置位时序寄存器 */
    /*
    CAN的四种工作模式
    正常模式

    正常模式下就是一个正常的 CAN 节点，可以向总线发送数据和接收数据。

    静默模式

    静默模式下，它自己的输出端的逻辑 0 数据会直接传输到它自己的输入端，逻辑 1 可以被发送到总线，所以它不能向总线发送显性位 (逻辑 0)，只能发送隐性位 (逻辑 1)。
    输入端可以从总线接收内容。由于它只可发送的隐性位不会强制影响总线的状态，所以把它称为静默模式。这种模式一般用于监测，它可以用于分析总线上的流量，但又不会因为发送显性位而影响总线。

    回环模式

    回环模式下，它自己的输出端的所有内容都直接传输到自己的输入端，输出端的内容同时也会被传输到总线上，即也可使用总线监测它的发送内容。
    输入端只接收自己发送端的内容，不接收来自总线上的内容。使用回环模式可以进行自检。

    回环静默模式

    回环静默模式是以上两种模式的结合，自己的输出端的所有内容都直接传输到自己的输入端，并且不会向总线发送显性位影响总线，不能通过总线监测它的发送内容。
    输入端只接收自己发送端的内容，不接收来自总线上的内容。这种方式可以在“热自检”时使用，即自我检查的时候，不会干扰总线。
    */
    /* 3.5.1 配置测试模式：环回静默模式 */
    CAN1->BTR |= (CAN_BTR_SILM | CAN_BTR_LBKM);

    /* 3.5.2 配置波特率 波特率分频器，用来定义Tq的长度：配置Tq的大小为1us
    tq = (BRP[9:0]+1) x tPCLK
    CAN1挂在APB1总线上，时钟频率是PCLK1 = 36MHz，则周期是1/36us，要配置tq的长度是1us，根据公式填入35
    Tq = 1us
    */
    CAN1->BTR &= ~CAN_BTR_BRP; // 先清除
    CAN1->BTR |= (35 << 0);

    /* 3.5.3 配置时间段1和时间段2的长度 BS1=3Tq BS2=6Tq
    Time segment 1
    tBS1 = tq x (TS1[3:0] + 1)

    Time segment 2
    tBS2 = tq x (TS2[2:0] + 1)
    */
    CAN1->BTR &= ~(CAN_BTR_TS1 | CAN_BTR_TS2); // 先清除
    CAN1->BTR |= (2 << 16);
    CAN1->BTR |= (5 << 20);

    /* 3.5.4 配置再同步的跳跃宽度 2Tq
    tRJW = tq x (SJW[1:0] + 1)
    */
    CAN1->BTR &= ~CAN_BTR_SJW;
    CAN1->BTR &= (1 << 24);

    /* 3.6 退出初始化模式 */
    CAN1->MCR &= ~CAN_MCR_INRQ;
    /* 等待退出初始化模式 */
    while (CAN1->MSR & CAN_MSR_INAK)
        ;

    /* 3.7 配置过滤器 */
    Driver_CAN_FilterConfig();
}

/**
 * @brief 配置CAN的标识符过滤器
 * @param
 */
void Driver_CAN_FilterConfig(void)
{
    /* 1.进入过滤器的初始化模式 */
    CAN1->FMR |= CAN_FMR_FINIT;

    /* 2.配置过滤器组0 工作模式为屏蔽位模式
    28 个过滤器组，每个过滤器组有 2 个32位寄存器

    0: Two 32-bit registers of filter bank x are in Identifier Mask mode.
    1: Two 32-bit registers of filter bank x are in Identifier List mode.
    */
    CAN1->FM1R &= ~CAN_FM1R_FBM0;

    /* 3.配置过滤器组0的位宽：
    1：单个32位
    0：将一个32位分为两个16位
    */
    CAN1->FS1R |= CAN_FS1R_FSC0;

    /* 4.给过滤器组0关联FIFO（应用到哪个接收队列）
    0：FIFO0
    1：FIFO1
    */
    CAN1->FFA1R &= ~CAN_FFA1R_FFA0;

    /* 5.配置过滤器组0的两个32位寄存器，如果配置成16位宽则是4各 */
    CAN1->sFilterRegister[0].FR1 = 0x00000000; // ID值
    CAN1->sFilterRegister[0].FR2 = 0x00000000; // 保留所有报文 1表示必须匹配 0表示不关心
    /* 针对16位宽 */
    // CAN1->sFilterRegister[0].FR1 = 0x0000 << 16; // ID值
    // CAN1->sFilterRegister[0].FR1 = 0x0000 << 0;  // 掩码值

    /* 6.激活过滤组0 */
    CAN1->FA1R |= CAN_FA1R_FACT0;

    /* 7.退出过滤器的初始化模式 */
    CAN1->FMR &= ~CAN_FMR_FINIT;
}

/**
 * @brief 发送消息
 * @param stdId 标准帧的标识符
 * @param data 要发送的数据
 * @param len 发送的数据的字节数
 */
void Driver_CAN_SendMsg(uint16_t stdId,
                        uint8_t *data,
                        uint8_t len)
{
    /* 判断发送的数据字节长度 */
    if (len > 8)
    {
        printf("数据长度不能超过8个字节\r\n");
        return;
    }

    /* 1.等待发送邮箱0 1 2其中一个为空
    0：非空
    1：空
    */
    uint8_t mailBoxIndex = 0;
    CAN_TxMailBox_TypeDef tmpMailBox;
    do
    {
        // 检查邮箱是否为空
        if (CAN1->TSR & (CAN_TSR_TME0 << mailBoxIndex))
        {
            // 找到不为空的邮箱，进行处理
            tmpMailBox = CAN1->sTxMailBox[mailBoxIndex];
            break;
        }

        // 更新索引
        mailBoxIndex++;
        if (mailBoxIndex >= 3)
        {
            mailBoxIndex = 0; // 重置索引
        }
    } while (1); // 持续循环直到找到不为空的邮箱

    /* 2.设置使用标准帧 0：标准帧 1：扩展帧*/
    /* 因为这里有3个邮箱，没法将标识位 CAN_TI0R_IDE 写死
        有三种解决方案

        方案一：定义一个数组，根据mailBoxIndex来选择使用哪个标识位，如下

        uint32_t CAN_TI_IDE[] = {CAN_TI0R_IDE, CAN_TI1R_IDE, CAN_TI2R_IDE};
        // 根据当前邮箱索引选择对应的标识位
        tmpMailBox.TIR &= ~CAN_TI_IDE[mailboxIndex];

        ===================================================
        方案二：使用分支，根据mailBoxIndex来选择使用哪个标识位，如下

        if MAILBOX_INDEX == 0
            tmpMailBox.TIR &= ~CAN_TI0R_IDE;
        elif MAILBOX_INDEX == 1
            tmpMailBox.TIR &= ~CAN_TI1R_IDE;
        elif MAILBOX_INDEX == 2
            tmpMailBox.TIR &= ~CAN_TI2R_IDE;
        else
            #error "Invalid MAILBOX_INDEX. It must be 0, 1, or 2."

        ===================================================
        方案三：根据mailBoxIndex进行寄存器的移位操作，因为寄存器跨度太大容易出错，不推荐使用
    */
    // tmpMailBox.TIR &= ~CAN_TI0R_IDE;
    uint32_t CAN_TIxR_IDE[] = {CAN_TI0R_IDE, CAN_TI1R_IDE, CAN_TI2R_IDE};
    tmpMailBox.TIR &= ~CAN_TIxR_IDE[mailBoxIndex];

    /* 3.配置数据帧或者远程帧 0：数据帧 1：远程帧（遥控帧） */
    // tmpMailBox.TIR &= ~CAN_TI0R_RTR;
    uint32_t CAN_TIxR_RTR[] = {CAN_TI0R_RTR, CAN_TI1R_RTR, CAN_TI2R_RTR};
    tmpMailBox.TIR &= ~CAN_TIxR_RTR[mailBoxIndex];

    /* 4.设置标准帧的11位ID（标识符） */
    tmpMailBox.TIR &= ~CAN_RI0R_STID;
    tmpMailBox.TIR |= (stdId << 21);

    /* 5.准备数据 */
    /* 5.1设置数据长度 */
    // tmpMailBox.TDTR &= ~CAN_TDT0R_DLC;
    uint32_t CAN_TDTxR_DLC[] = {CAN_TDT0R_DLC, CAN_TDT1R_DLC, CAN_TDT2R_DLC};
    tmpMailBox.TDTR &= ~CAN_TDTxR_DLC[mailBoxIndex];

    /*
    数据长度代码 此字段定义数据帧包含的数据字节数或远程帧请求。
    一条消息可以包含 0 到 8 个数据字节，具体取决于 DLC 字段中的值。
    */
    tmpMailBox.TDTR |= (len << 0);

    /* 5.2 在报文中设置具体的数据 TDLR TDHR */
    tmpMailBox.TDLR = 0; // 先清空
    tmpMailBox.TDHR = 0;

    for (uint8_t i = 0; i < len && i < 8; i++) // 限制len最大为8
    {
        if (i < 4)
        {
            tmpMailBox.TDLR |= (data[i] << (i * 8));
        }
        else
        {
            tmpMailBox.TDHR |= (data[i] << ((i - 4) * 8));
        }
    }

    /* 6.开始发送数据
    Transmit mailbox request
    Set by software to request the transmission for the corresponding mailbox.
    Cleared by hardware when the mailbox becomes empty
    */
    // tmpMailBox.TIR |= CAN_TI0R_TXRQ;
    uint32_t CAN_TIxR_TXRQ[] = {CAN_TI0R_TXRQ, CAN_TI1R_TXRQ, CAN_TI2R_TXRQ};
    tmpMailBox.TIR |= CAN_TIxR_TXRQ[mailBoxIndex];
}

/**
 * @brief
 * @param fifox 指定接收哪个FIFO中的消息 FIFO0 FIFO1
 * @param rxDataStruct 将从FIFOx中接受到的消息封装到数组中
 * @param rxMsgCount FIFOx中的消息数量
 */
void Driver_CAN_ReceiveMsg(enum FIFOx fifox, RxDataStruct *rxDataStruct, uint8_t *rxMsgCount)
{
    switch (fifox)
    {
    case FIFO0:
        /* 1.获取当前 FIFO0 中的报文（消息）数 有两个FIFO 每个FIFOx中有三个邮箱，对多缓存三个报文
        通过检查FMP的值，软件可以知道有多少条消息在等待被处理。
        如果FMP的值为0，则表示没有待处理的消息。
        如果FMP的值大于0，软件应及时读取和处理这些消息，以防止FIFO溢出。

        CAN receive FIFO 0 register (CAN_RF0R)
        */
        *rxMsgCount = (CAN1->RF0R & CAN_RF0R_FMP0) >> 0;

        RxDataStruct *tmpMsg;
        for (uint8_t i = 0; i < *rxMsgCount; i++)
        {
            tmpMsg = rxDataStruct + i * sizeof(RxDataStruct);
            /* 2.读取消息中的标准标识符数据

            CAN receive FIFO mailbox identifier register (CAN_RIxR) (x=0..1)
            */
            tmpMsg->stdId = ((CAN1->sFIFOMailBox[0].RIR & CAN_RI0R_STID) >> 21) & 0x07ff;

            /* 3.读取数据长度

            CAN receive FIFO mailbox data length control and time stamp register (CAN_RDTxR) (x=0..1)
            */
            tmpMsg->len = (CAN1->sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC) & 0x0f;

            /* 4.读取数据 */
            for (uint8_t j = 0; j < tmpMsg->len; j++)
            {
                if (j < 4)
                {
                    tmpMsg->data[j] = (CAN1->sFIFOMailBox[0].RDLR >> (j * 8)) & 0xff;
                }
                else
                {
                    tmpMsg->data[j] = (CAN1->sFIFOMailBox[0].RDHR >> ((j - 4) * 8)) & 0xff;
                }
            }

            /* 释放邮箱，CAN_RF0R_FMP0 会减1 */
            CAN1->RF0R |= CAN_RF0R_RFOM0;
        }
        break;
    default: // 其他的视为FIFO1

        /* 1.获取当前 FIFO0 中的报文（消息）数 有两个FIFO 每个FIFOx中有三个邮箱，对多缓存三个报文
        通过检查FMP的值，软件可以知道有多少条消息在等待被处理。
        如果FMP的值为0，则表示没有待处理的消息。
        如果FMP的值大于0，软件应及时读取和处理这些消息，以防止FIFO溢出。

        CAN receive FIFO 1 register (CAN_RF1R)
        */
        *rxMsgCount = (CAN1->RF1R & CAN_RF1R_FMP1) >> 0;

        RxDataStruct *tmpMsg;
        for (uint8_t i = 0; i < *rxMsgCount; i++)
        {
            tmpMsg = rxDataStruct + i * sizeof(RxDataStruct);
            /* 2.读取消息中的标准标识符数据

            CAN receive FIFO mailbox identifier register (CAN_RIxR) (x=0..1)
            */
            tmpMsg->stdId = ((CAN1->sFIFOMailBox[1].RIR & CAN_RI1R_STID) >> 21) & 0x07ff;

            /* 3.读取数据长度

            CAN receive FIFO mailbox data length control and time stamp register (CAN_RDTxR) (x=0..1)
            */
            tmpMsg->len = (CAN1->sFIFOMailBox[1].RDTR & CAN_RDT1R_DLC) & 0x0f;

            /* 4.读取数据 */
            for (uint8_t j = 0; j < tmpMsg->len; j++)
            {
                if (j < 4)
                {
                    tmpMsg->data[j] = (CAN1->sFIFOMailBox[1].RDLR >> (j * 8)) & 0xff;
                }
                else
                {
                    tmpMsg->data[j] = (CAN1->sFIFOMailBox[1].RDHR >> ((j - 4) * 8)) & 0xff;
                }
            }

            /* 释放邮箱，CAN_RF1R_FMP1 会减1 */
            CAN1->RF1R |= CAN_RF1R_RFOM1;
        }

        break;
    }
}
