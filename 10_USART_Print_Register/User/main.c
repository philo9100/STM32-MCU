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
	
	int a = 10;

	while (1)
	{	
		printf("Hello World!\r\n");
		Delay(1000);

		printf("a = %d\r\n", a); // 会发送a = 10\r\n

		Driver_USART1_SendChar(a); // 会发送0x0A

		sprintf(buff, "a = %d\r\n", a);	// 将a的值转换为字符串，存放在buff中
		Driver_USART1_SendString(buff, 8); // 发送buff中的字符串 a = 10\r\n	等价于printf("a = %d\r\n", a);
	}
	

	return 0;
}
