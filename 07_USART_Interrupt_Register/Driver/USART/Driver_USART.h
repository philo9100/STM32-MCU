#ifndef __DRIVER_USART_H
#define __DRIVER_USART_H

#include "stm32f10x.h"

void Driver_USART1_Init(void);

void Driver_USART1_SendChar(uint8_t byte);

void Driver_USART1_SendString(uint8_t *str, uint32_t len);

uint8_t Driver_USART1_ReceiveByte(void);

void Driver_USART1_ReceiveString(uint8_t *buff, uint8_t *len);



#endif

