#include "Driver_USART.h" // Device header
#include "Driver_LED.h"	  // Device header
#include "Delay.h"
#include "Driver_ADC.h"

uint16_t data[2];

int main()
{

	Driver_USART1_Init();

	printf("ADC双通道转换通过DMA传输实验开始...\r\n");

	Driver_ADC1_Init();
	Driver_ADC1_DMA_Init();
	Driver_ADC1_DMA_Start((uint32_t)data, 2);

	double RheostatV, PowerV;

	while (1)
	{
		RheostatV = (data[0] * 3.3) / 4096;
		PowerV = (data[1] * 3.3) / 4096;

		printf("RheostatV = %.2f, PowerV = %.2f\r\n", \
				RheostatV, \
				PowerV);

		Delay_ms(1000);
	}

	return 0;
}
