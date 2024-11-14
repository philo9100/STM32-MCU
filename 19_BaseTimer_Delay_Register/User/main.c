#include "Driver_USART.h"                  // Device header
#include "Driver_LED.h"                   // Device header
#include "Delay.h"
#include "Driver_TIM6.h"

int main()
{

	Driver_USART1_Init();
	Driver_LED_Init();
	printf("基本定时器实验开始...\r\n");
	//Driver_TIM6_Init();

	Delay_ms_BaseTimer(2000);


	while (1)
	{	

	}
	
	return 0;
}
