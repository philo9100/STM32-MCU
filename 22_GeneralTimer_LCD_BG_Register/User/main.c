#include "Driver_USART.h"

#include "Delay.h"
#include "Driver_SysTick.h"
#include "Driver_Key.h"
#include "Driver_TIM3.h"

int main()
{
    Driver_USART1_Init();
    printf("实验开始...\r\n");

    Driver_Key3_Init();
    Driver_Key4_Init();

    Driver_TIM3_Init();
    Driver_TIM3_Start();


    int8_t dutyCycle = 50;
    Driver_TIM3_SetDutyCycle(dutyCycle);
    while (1)
    {
        if (isKey3Pressed)
        {
            isKey3Pressed = 0;
            dutyCycle += 5;
            dutyCycle = dutyCycle >= 100 ? 100 : dutyCycle;
            Driver_TIM3_SetDutyCycle(dutyCycle);
            printf("3=%d\r\n",dutyCycle);
            
        }
        if (isKey4Pressed)
        {
            isKey4Pressed = 0;
            dutyCycle -= 5;
            dutyCycle = dutyCycle <= 0 ? 0 : dutyCycle;
            Driver_TIM3_SetDutyCycle(dutyCycle);
            printf("4=%d\r\n",dutyCycle);
        }
    }
}
