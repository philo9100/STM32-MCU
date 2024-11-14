#ifndef __INF_W5500_HTTPSERVER_H
#define __INF_W5500_HTTPSERVER_H

#include "stdio.h"
#include "string.h"
#include "socket.h"
#include "w5500.h"
#include "httpServer.h"

#include "Driver_LED.h"

void Inf_W5500_HTTPServerInit(void);

void Inf_W5500_HTTPServerStart(void);

void Inf_W5500_DoAction(uint8_t *url);

#endif

