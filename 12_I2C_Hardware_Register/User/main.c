#include "Driver_I2C2.h"                  // Device header
#include "string.h"
#include "Inf_W24C02.h"


int main()
{

	Driver_USART1_Init();

	printf("尚硅谷 I2C 硬件 模拟实验\r\n");

	Inf_W24C02_Init();

	/****************************************************** */
	// Inf_W24C02_WriteByte(0x00, 'x');
	// Inf_W24C02_WriteByte(0x01, 'y');
	// Inf_W24C02_WriteByte(0x02, 'z');
	// Delay_ms(5);
	// uint8_t byte1 = Inf_W24C02_ReadByte(0x00);
	// uint8_t byte2 = Inf_W24C02_ReadByte(0x01);
	// uint8_t byte3 = Inf_W24C02_ReadByte(0x02);

	// printf("%c\r\n%c\r\n%c\r\n", byte1, byte2, byte3);

	/******************************************************* */
	// Inf_W24C02_WritePage(0x00, "123456", 6);
	// uint8_t byte3 = Inf_W24C02_ReadByte(0x02);
	// printf("%c\r\n", byte3);
	// printf("%c\r\n", Inf_W24C02_ReadByte(0x05));

	/******************************************************* */
	uint8_t buff[10] = {0};
	Inf_W24C02_ReadPage(0x00, buff, 6);
	printf("%s\r\n",buff);


	/* 清空缓冲区 */
	memset(buff, 0, sizeof(buff));

	Inf_W24C02_WritePage(0x00, "abc", 3);
	Inf_W24C02_ReadPage(0x00, buff, 6);
	printf("%s\r\n",buff);


	while (1)
	{	
		
	}
	

	return 0;
}
