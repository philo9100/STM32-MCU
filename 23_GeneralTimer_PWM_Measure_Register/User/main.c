#include "Driver_USART.h"                  // Device header
#include "Driver_LED.h"                   // Device header
#include "Delay.h"
#include "Driver_TIM5.h"
#include "Driver_TIM4.h"

int main()
{

	Driver_USART1_Init();

	Driver_TIM5_Init();
	Driver_TIM4_Init();

	/* TIM5产生PWM */
	Driver_TIM5_Generate();
	/* TIM4测量TIM5产生的PWM的频率和周期 */
	Driver_TIM4_Generate();

	double t, f;

	while (1)
	{
		t = Driver_TIM4_GetPWMCycle();
		f = Driver_TIM4_GetPWMFrequency();

		printf("t = %.2f, f = %.2f\r\n", t, f);
		// 打印结果和TIM5设置的一致，周期是10ms，频率是100Hz
 
		Delay_ms(1000);
	}
	
	return 0;
}
