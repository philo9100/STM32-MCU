#include "stm32f10x.h"                  // Device header
#include "Driver_USART.h"                  // Device header
#include "Delay.h"
#include "string.h"

extern uint8_t buff[100];
extern uint8_t len;

extern uint8_t isSend;


int main()
{

	Driver_USART1_Init();
	
	/* 直接使中断方式接收数据，不用再轮询等待
	Driver_USART1_SendChar('A\r\n');
	uint8_t *str = "Hello World\r\n";
	*/
	while (1)
	{	/*
		Delay_s(1);
		Driver_USART1_SendChar('B\r\n');
		Driver_USART1_SendString(str, strlen((char*)str));

		uint8_t c = Driver_USART1_ReceiveByte();
		Driver_USART1_SendChar(c);

		Driver_USART1_ReceiveString(buff, &len);
		Driver_USART1_SendString(buff, len);
		*/
		if (isSend)
		{
			Driver_USART1_SendString(buff, len);
			len = 0;
			isSend = 0;
		}
				
	}
	

	return 0;
}
