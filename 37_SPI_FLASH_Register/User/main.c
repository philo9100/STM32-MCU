#include "Driver_USART.h" // Device header
#include "Driver_LED.h"	  // Device header
#include "Delay.h"
#include "Driver_ADC.h"
#include "Inf_W25Q32.h"

uint16_t data[2];

int main()
{

	Driver_USART1_Init();
	Inf_W25Q32_Init();

	printf("SPI实验开始...\r\n");

	/* 读取id测试是否正常 */
	uint8_t mid = 0;
	uint16_t did = 0;

	Inf_W25Q32_ReadId(&mid, &did);

	printf("mid = 0x%02x\r\ndid = 0x%04x\r\n", mid, did);
	
	/* 先擦除要写入的扇区(0,0) */
	Inf_W25Q32_EraseSector(0, 0);
	
	uint8_t *data = "abcd";

	Inf_W25Q32_WritePage(0, 0, 0, data, 4);
	

	uint8_t buff[10] = {0};

	Inf_W25Q32_Read(0, 0, 0, buff, 4);

	printf("%s\r\n",buff);

	while (1)
	{
	}

	return 0;
}
