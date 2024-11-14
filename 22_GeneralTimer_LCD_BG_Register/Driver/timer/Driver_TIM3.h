#ifndef __DRIVER_TIM3_H
#define __DRIVER_TIM3_H
#include "stm32f10x.h"


void Driver_TIM3_Init(void);

void Driver_TIM3_Start(void);

void Driver_TIM3_Stop(void);

void Driver_TIM3_SetDutyCycle(uint8_t dutyCycle);

#endif

