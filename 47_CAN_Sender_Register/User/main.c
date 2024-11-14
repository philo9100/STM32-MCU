#include "stm32f10x.h"	  // Device header
#include "Driver_USART.h" // Device header
#include "Delay.h"
#include "string.h"
#include "Driver_CAN.h"
#include "string.h"

RxDataStruct rxDataStruct[3];
uint8_t rxMsgCount = 0;
uint8_t buff[100];

int main()
{

	Driver_USART1_Init();

	printf("CAN 通讯实验...\r\n");

	Driver_CAN_Init();

	uint32_t i = 0;
	
	while (1)
	{
		i++;
		sprintf((char*)buff, "ab%d\r\n", i);
		Driver_CAN_SendMsg(0x110, buff, strlen(buff));
		Delay_ms(500);
	}

	return 0;
}
