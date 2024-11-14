#include "Driver_USART.h"                  // Device header
#include "Driver_LED.h"                   // Device header
#include "Delay.h"
#include "Driver_DMA.h"

const uint8_t src[] = {10, 20, 30, 40}; 
uint8_t des[4] = {0};


int main()
{

	Driver_USART1_Init();

	printf("DMA实验 ROM->RAM 开始...\r\n");
	
	Driver_DMA1_Init();

	Driver_DMA1_TransimitData((uint32_t)src, (uint32_t)des, 4);

	while (isTransmitFinished == 0)
	{
		printf("%d, %d, %d, %d\r\n", des[0], des[1], des[2], des[3]);
	}

	while (1)
	{
		Delay_ms(1000);
	}
	
	return 0;
}
