#include "Driver_USART.h"                  // Device header
#include "Driver_LED.h"                   // Device header
#include "Delay.h"
#include "Driver_TIM5.h"
#include "Driver_TIM1.h"

int main()
{

	Driver_USART1_Init();

	printf("高级定时器实验开始...\r\n");
	
	Driver_TIM1_Init();

	Driver_TIM1_Generate();

	while (1)
	{
	
		Delay_ms(1000);
	}
	
	return 0;
}
