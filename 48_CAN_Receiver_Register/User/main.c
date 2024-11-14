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

	printf("CAN 通讯实验...\r\n");

	Driver_CAN_Init();

	while (1)
	{
		/* 读取 */
		Driver_CAN_ReceiveMsg(rxDataStruct, &rxMsgCount);

		for (uint8_t i = 0; i < rxMsgCount; i++)
		{
			RxDataStruct rds = rxDataStruct[i];
			printf("stdId: %d, len: %d, data: ", rds.stdId, rds.len);
			
		}
		

	}

	return 0;
}
