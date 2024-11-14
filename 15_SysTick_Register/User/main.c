#include "Delay.h"
#include "Driver_USART.h" // Device header
#include "Driver_LED.h"	  // Device header
#include "Driver_SysTick.h"

int main()
{

	Driver_USART1_Init();
	printf("系统定时器实验开始...\r\n");
	Driver_LED_Init();
	Driver_SysTick_Init();

	while (1)
	{
	}

	return 0;
}
