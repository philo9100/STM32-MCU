#include "Driver_USART.h" // Device header
#include "Driver_LED.h"	  // Device header
#include "Delay.h"
#include "Driver_DMA.h"

uint8_t src[] = {10, 20, 30, 40};

int main()
{

	Driver_USART1_Init();

	printf("DMA实验 RAM->USART 开始...\r\n");

	Driver_DMA1_Init();

	Driver_DMA1_TransimitData((uint32_t)src, (uint32_t)(&(USART1->DR)), 4);

	while (1)
	{
		Delay_ms(1000);
	}

	return 0;
}
