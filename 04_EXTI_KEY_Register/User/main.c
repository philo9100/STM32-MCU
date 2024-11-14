#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Driver_KEY.h"
#include "Driver_LED.h"


int main()
{
	/* 1.初始化LED */
	Driver_LED_Init();

	/* 2.初始化KEY */
	Driver_KEY_Init();

	while(1)
	{
	
	}

	return 0;
}
  