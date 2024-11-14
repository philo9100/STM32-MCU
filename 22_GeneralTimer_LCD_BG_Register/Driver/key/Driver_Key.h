#ifndef __DRIVER_KEY_H
#define __DRIVER_KEY_H

#include "stm32f10x.h"

extern uint8_t isKey3Pressed;
extern uint8_t isKey4Pressed;
void Driver_Key3_Init(void);
void Driver_Key4_Init(void);
#endif
