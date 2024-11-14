#ifndef __DRIVER_TIM4_H
#define __DRIVER_TIM4_H

#include "stm32f10x.h"

void Driver_TIM4_Init(void);

void Driver_TIM4_Generate(void);

void Driver_TIM4_Stop(void);

double Driver_TIM4_GetPWMCycle(void);

double Driver_TIM4_GetPWMFrequency(void);

double Driver_TIM4_GetDutyCycle(void);

#endif

