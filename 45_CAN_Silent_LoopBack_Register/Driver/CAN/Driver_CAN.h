#ifndef __DRIVER_CAN_H
#define __DRIVER_CAN_H

#include "stm32f10x.h"
#include "Driver_USART.h"

enum FIFOx
{
    FIFO0,
    FIFO1
};

/**
 * @brief CAN用来接收消息的结构体，用来描述收到的报文信息
 */
typedef struct
{
    uint16_t stdId;
    uint8_t data[8];    // 一个消息最多有8个字节的数据
    uint8_t len;
} RxDataStruct;

void Driver_CAN_Init(void);

void Driver_CAN_SendMsg(uint16_t stdId, uint8_t *data, uint8_t len);

void Driver_CAN_ReceiveMsg(enum FIFOx fifox, RxDataStruct *rxDataStruct, uint8_t *rxMsgCount);

#endif

