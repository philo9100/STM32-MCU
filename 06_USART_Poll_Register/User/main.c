#include "Driver_USART.h" // Device header
#include "Delay.h"
#include "string.h"

int main()
{
	Driver_USART1_Init();
	Driver_USART1_SendChar('A\r\n');
	uint8_t *str = "Hello World\r\n";
	while (1)
	{
		Delay_s(1);
		Driver_USART1_SendChar('B\r\n');
		Driver_USART1_SendString(str, strlen((char *)str));

		uint8_t c = Driver_USART1_ReceiveByte();
		Driver_USART1_SendChar(c);

		Driver_USART1_ReceiveString(buff, &len);
		Driver_USART1_SendString(buff, len);
	}

	return 0;
}
