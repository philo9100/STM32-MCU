#include "Driver_USART.h"                  // Device header
#include "Driver_LED.h"                   // Device header
#include "Delay.h"
#include "Driver_TIM5.h"

int main()
{

	Driver_USART1_Init();
	Driver_LED_Init();

	Driver_TIM5_Init();
	Driver_TIM5_Generate();

	/* 设置初始占空比 */
	uint8_t dutyCycle = 0;
	Driver_TIM5_SetDutyCycle(dutyCycle);

	while (1)
	{
		breathingLight(dutyCycle);
		Delay_ms_BaseTimer(50);	
	}
	
	return 0;
}

void breathingLight(uint8_t dutuCycle)
{
	if (dutuCycle < 100 && dutuCycle >= 0)
	{
		dutuCycle += 5;
		Driver_TIM5_SetDutyCycle(dutuCycle);
		breathingLight(dutuCycle);
	}

	dutuCycle -= 5;
	if (dutuCycle > 0)
	{
		Driver_TIM5_SetDutyCycle(dutuCycle);
	}
}