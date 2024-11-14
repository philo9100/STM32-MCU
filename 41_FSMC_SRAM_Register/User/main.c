#include "Driver_USART.h"

#include "Delay.h"
#include "Driver_FSMC.h"
#define I4 (uint8_t *)0x68000010
/* 定义一个变量,存储到外置的SRAM中 */;
/* 方式1: 使用关键词 __attribute__  at*/
uint8_t v1 __attribute__((at(0x68000000)));
uint8_t v2 __attribute__((at(0x68000004)));
uint16_t i1 = 20;
int main()
{
    uint8_t v3 __attribute__((at(0x68000007)));
    Driver_USART1_Init();
    printf("扩展内存\r\n");
    Driver_FSMC_Init();
    v1 = 200;
    v2 = 100;
    v3=11;

    /* 方式2: 定义指针 */
    *(uint8_t *)0x68000001 = 30;
    printf("0x68000001=%d\r\n", *(uint8_t *)0x68000001);

    printf("v1=%p, %d\r\n", &v1, v1);
    printf("v2=%p, %d\r\n", &v2, v2);
    printf("i1=%p, %d\r\n", &i1, i1);
    printf("v3=%p, %d\r\n", &v3, v3);

    *I4 = 22;
    printf("I4=%p, %d\r\n", I4, *I4);

    while (1)
    {
    }
}
