#include "Inf_W5500.h"


void Inf_W5500_Init(void)
{
    /* 1.给w5500注册底层函数 */

    /* 2.初始化SPI 和 GPIO */
    Driver_SPI_Init();
    Driver_GPIO_Init();

    /* 3.软件重启W5500 */
    Inf_W5500_ReStart();

    /* 4.设置物理地址：MAC地址 */
    Inf_W5500_SetMAC();

    /* 5.设置IP地址 */
    Inf_W5500_SetIP();
}

/**
 * @brief IP地址
 */
uint8_t ip[] = {192, 168, 31, 222};

/* 子网掩码 */
uint8_t mask[] = {255, 255, 255, 0};

/* 网关 */
uint8_t gateWay[] = {192, 168, 31, 1};

void Inf_W5500_SetIP(void)
{
    printf("开始 设置IP地址\r\n");
    /* 设置 */
    setSIPR(ip);
    setSUBR(mask);
    setGAR(gateWay);    
    printf("结束 设置IP地址\r\n");
}


uint8_t mac[] = {110, 120, 130, 140, 150, 160};
void Inf_W5500_SetMAC(void)
{
    printf("开始 设置MAC地址\r\n");
    setSHAR(mac);
    printf("结束 设置MAC地址\r\n");
}

void Inf_W5500_ReStart(void)
{
    RST_L;
    Delay_ms(1000);     // 重启引脚拉低至少500ms可以时序芯片的重启
    RST_H;
    Delay_ms(500);
}



