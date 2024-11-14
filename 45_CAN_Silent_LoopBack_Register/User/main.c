#include "stm32f10x.h"	  // Device header
#include "Driver_USART.h" // Device header
#include "Delay.h"
#include "string.h"
#include "Driver_CAN.h"

RxDataStruct rxDataStruct[3];
uint8_t rxMsgCount = 0;

int main()
{

	Driver_USART1_Init();

	printf("CAN 通讯实验: 静默回环 寄存器版\r\n");

	Driver_CAN_Init();

	printf("CAN 初始化配置完成...\r\n");

	uint16_t stdId = 0x066;
	uint8_t *tData = "abcdefg";
	
	Driver_CAN_SendMsg(stdId, tData, strlen((char *)tData));
	printf("发送完毕...\r\n");

	tData = "123";
	CAN_SendMsg(stdId, tData, strlen((char *)tData));
	printf("发送完毕...\r\n");

	stdId = 0x067;
	tData = "xyz";
	CAN_SendMsg(stdId, tData, strlen((char *)tData));
	printf("发送完毕...\r\n");

	/* 1. 接收数据 */
	
	Driver_CAN_ReceiveMsg(FIFO0, rxDataStruct, &rxMsgCount);
	printf("接收完毕 rxMsgCount = %d...\r\n", rxMsgCount);

	/* 2. 输出消息 */
	uint8_t i;
	for (i = 0; i < rxMsgCount; i++)
	{
		RxDataStruct msg = rxDataStruct[i];
		printf("stdId = %d, length = %d, msgData = %s\r\n", msg.stdId, msg.len, msg.data);
	}
	Delay_ms(1000);

	while (1)
	{
	}

	return 0;
}
