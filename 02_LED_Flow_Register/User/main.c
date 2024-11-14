#include "stm32f10x.h"                  // Device header
#include "Driver_LED.h"
#include "Delay.h"

int main()
{
	uint16_t leds[] = {LED_1, LED_2, LED_3};

	/* 1.初始化LED */
	Driver_LED_Init();	

	/* 2.打开所有LED */
	Driver_LED_OnAll(leds, 3);

	Driver_LED_ON(LED_2);

	Driver_LED_OffAll(leds, 3);
	
	/* 使用 keil 进行 debug 需要调试器 */
	
	while (1)
	{
		// 流水灯
		for (uint8_t i = 0; i < 3; i++)
		{
			Driver_LED_OffAll(leds, 3);
			Driver_LED_ON(leds[i]);
			Delay_ms(500);
		}
		Driver_LED_OffAll(leds, 3);
		Driver_LED_ON(leds[1]);
		Delay_ms(500);
	}
	

	return 0;
}
  