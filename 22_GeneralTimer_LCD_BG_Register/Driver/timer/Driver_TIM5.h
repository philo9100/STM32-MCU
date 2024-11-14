#ifndef __DRIVER_TIM5_H
#define __DRIVER_TIM5_H
#include "stm32f10x.h"


void Driver_TIM5_Init(void);

void Driver_TIM5_Start(void);

void Driver_TIM5_Stop(void);

void Driver_TIM5_SetDutyCycle(uint8_t dutyCycle);

#endif

