#include "Driver_USART.h" // Device header
#include "Driver_LED.h"	  // Device header
#include "Delay.h"
#include "Inf_W5500.h"
#include "Inf_W5500_UDP.h"

int main()
{
	Driver_USART1_Init();

	printf("Ethernet lab start...\r\n");

	Inf_W5500_Init();

	while (1)
	{
		Inf_W5500_UDP();
	}

	return 0;
}
