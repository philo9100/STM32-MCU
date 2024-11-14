#include "Driver_USART.h" // Device header
#include "Driver_LED.h"	  // Device header
#include "Delay.h"
#include "Driver_ADC.h"

uint8_t src[] = {10, 20, 30, 40};

int main()
{

	Driver_USART1_Init();

	printf("ADC单通道转换实验开始...\r\n");

	Driver_ADC1_Init();

	Driver_ADC1_StartConvert();
	
	double v;

	while (1)
	{
		v = Driver_ADC1_ReadV();

		printf("v = %.2f\r\n", v);

		Delay_ms(1000);
	}

	return 0;
}
