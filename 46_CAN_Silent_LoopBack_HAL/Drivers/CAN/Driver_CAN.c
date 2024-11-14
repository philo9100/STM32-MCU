#include "Driver_CAN.h"

/**
 * @brief 初始化 CAN通讯
 * @param
 */
void Driver_CAN_Init(void)
{
    MX_CAN_Init();
    
    Driver_CAN_FilterConfig();
    
    /* 启动CAN，退出初始化 */
    HAL_CAN_Start(&hcan);
}

/**
 * @brief 配置CAN的标识符过滤器
 * @param
 *
 *
uint32_t FilterIdHigh:
作用：指定滤波器的标识符高位（MSB），用于32位配置时的标识符。对于16位配置，表示第一个标识符。
范围：0x0000 到 0xFFFF。

uint32_t FilterIdLow:
作用：指定滤波器的标识符低位（LSB），用于32位配置时的标识符。对于16位配置，表示第二个标识符。
范围：0x0000 到 0xFFFF。

uint32_t FilterMaskIdHigh:
作用：指定滤波器的掩码高位，用于过滤标识符的匹配。该成员用于32位配置时的掩码；在16位配置时为第一个掩码。
范围：0x0000 到 0xFFFF。

uint32_t FilterMaskIdLow:
作用：指定滤波器的掩码低位，用于过滤标识符的匹配。该成员用于32位配置时的掩码；在16位配置时为第二个掩码。
范围：0x0000 到 0xFFFF。

uint32_t FilterFIFOAssignment:
作用：指定将分配给此滤波器的FIFO（0或1）。可以用来决定接收到的消息将进入哪个FIFO。
取值：@ref CAN_filter_FIFO中的值。

uint32_t FilterBank:
作用：指定要初始化的滤波器库。对于单个CAN实例，范围为0到13；对于双CAN实例，范围为0到27。
范围：依赖于具体硬件的滤波器库配置。

uint32_t FilterMode:
作用：指定要初始化的滤波器模式。可以是标识符列表模式或掩码模式。
取值：@ref CAN_filter_mode中的值。

uint32_t FilterScale:
作用：指定滤波器的规模，可以选择16位或32位配置。
取值：@ref CAN_filter_scale中的值。

uint32_t FilterActivation:
作用：启用或禁用该滤波器。
取值：@ref CAN_filter_activation中的值。

uint32_t SlaveStartFilterBank:
作用：对于双CAN实例，指定从哪个滤波器库开始分配给从设备的滤波器。通常用于配置从设备的CAN滤波器。
 *
 *
 */
void Driver_CAN_FilterConfig(void)
{
    CAN_FilterTypeDef sFilter;
    sFilter.FilterBank = 0;                          /* 受用过滤器组0 */
    sFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0; /* 过滤器组0绑定到fifo0 */
    sFilter.FilterMode = CAN_FILTERMODE_IDMASK;      /* 过滤器模式：掩码模式 */
    sFilter.FilterScale = CAN_FILTERSCALE_32BIT;     /* 过滤器的位宽 */
    sFilter.FilterIdHigh = 0x0;
    sFilter.FilterIdLow = 0x0;
    sFilter.FilterMaskIdHigh = 0x0;
    sFilter.FilterMaskIdLow = 0x0;
    sFilter.FilterActivation = CAN_FILTER_ENABLE; /* 激活过滤器组0 */

    HAL_CAN_ConfigFilter(&hcan, &sFilter);
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
    /* 判断是否有空的发送邮箱 */
    while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) /* 返回的是空闲的发送邮箱的个数 */
        ;

    CAN_TxHeaderTypeDef sTxHeader;
    uint32_t pTxMailbox;
    sTxHeader.StdId = stdId;
    sTxHeader.IDE = CAN_ID_STD;
    sTxHeader.RTR = CAN_RTR_DATA;
    sTxHeader.DLC = len;

    HAL_CAN_AddTxMessage(&hcan, &sTxHeader, data, &pTxMailbox);
}


/**
 * @brief
 * @param fifox 指定接收哪个FIFO中的消息 FIFO0 FIFO1
 * @param rxDataStruct 将从FIFOx中接受到的消息封装到数组中
 * @param rxMsgCount FIFOx中的消息数量
 */
void Driver_CAN_ReceiveMsg(RxDataStruct *rxDataStruct, uint8_t *rxMsgCount)
{
    /* 获取目前 fifo中的收到的报文数 */
    *rxMsgCount = HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0);

    /* 遍历出所有的报文 */
    CAN_RxHeaderTypeDef rxHeader;
    for (uint8_t i = 0; i < *rxMsgCount; i++)
    {
        /* 接收FIFO中的报文的具体数据
            获取到的元数据信息会存储到 rxHeader 中
        */
        HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxHeader, rxDataStruct[i].data);
        rxDataStruct[i].stdId =rxHeader.StdId;
        rxDataStruct[i].len = rxHeader.DLC;
    }
}
