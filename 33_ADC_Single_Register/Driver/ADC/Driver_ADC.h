#ifndef __DRIVER_ADC_H
#define __DRIVER_ADC_H

#include "stm32f10x.h"

void Driver_ADC1_Init(void);

void Driver_ADC1_StartConvert(void);

double Driver_ADC1_ReadV(void);

#endif

