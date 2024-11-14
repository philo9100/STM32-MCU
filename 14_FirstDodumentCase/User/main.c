#include "Driver_USART.h"
#include "Inf_W24C02.h"
#include "Driver_Key.h"
#include "Driver_LED.h"
#include "string.h"

#define E2PROM_START_ADDR 0x00

extern uint8_t buff[CMD_LEN];
extern uint8_t isToSend;
extern uint8_t isKeypressed;
uint8_t cmd[3][2];
uint8_t index = 0;

void handleCMD(void);
void doLedFlow(void);
void checkKey(void);
void checkUsart(void);
int main()
{
    Driver_USART1_Init();
    printf("尚硅谷 I2C 硬件 实验开始....\r\n");
    Driver_LED_Init();
    Driver_Key_Init();
    Inf_W24C02_Init();

    /* 1. 先从E2PROM中读取指令 */
    Inf_W24C02_ReadBytes(E2PROM_START_ADDR, buff, CMD_LEN);
    /* 2. 如果没有读取到指令, 等待串口下发指令 */
    if (strstr((char *)buff, "#") == NULL)
    {
        printf("从E2PROM中没有读到指令...\r\n");
        /* 2.1 等待串口下发指令 */
        printf("等待串口下发指令...\r\n");
        while (!isToSend)
            ;
        isToSend = 0;
        printf("下发的指令是: %s\r\n", buff);
        /* 2.3 把指令字符串写入到E2PROM */
        Inf_W24C02_WriteBytes(E2PROM_START_ADDR, buff, CMD_LEN);
    }
    /* 3. 获取到了指令 解析指令. 把解析出来的数字存储到一个二维度数组中 */
    printf("开始解析指令... %s \r\n", buff);
    handleCMD();
    while (1)
    {
        // 4. 流水灯显示
        doLedFlow();
        // 5. 检测按键
        checkKey();
        // 6. 检测串口
        checkUsart();
    }
}

// 处理命令
void handleCMD(void)
{
    /* 1,3#3,2#2,1 */
    sscanf((char *)buff, "%hhu,%hhu#%hhu,%hhu#%hhu,%hhu",
           &cmd[0][0], &cmd[0][1],
           &cmd[1][0], &cmd[1][1],
           &cmd[2][0], &cmd[2][1]);

    for (uint8_t i = 0; i < 3; i++)
    {
        for (uint8_t j = 0; j < 2; j++)
        {
            printf("%d\t", cmd[i][j]);
        }
        printf("\r\n");
    }
}

void doLedFlow(void)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        /* 先关闭所有的LED灯 */
        Drviver_LED_Off(LED_1 | LED_2 | LED_3);
        Delay_ms(500);

        if (cmd[index][i] == 1)
        {
            Drviver_LED_On(LED_1);
        }
        else if (cmd[index][i] == 2)
        {
            Drviver_LED_On(LED_2);
        }
        else if (cmd[index][i] == 3)
        {
            Drviver_LED_On(LED_3);
        }
        Delay_ms(500);
    }
}

void checkKey(void)
{
    if (isKeypressed)
    {
        isKeypressed = 0;
        index++;
        index %= 3;
        printf("你按下了按键: %d\r\n", index);
    }
}

void checkUsart(void)
{
    if (isToSend)
    {
        printf("下发的新的命令: %s\r\n", buff);
        isToSend = 0;
        handleCMD();
        Inf_W24C02_WriteBytes(E2PROM_START_ADDR, buff, CMD_LEN);
    }
}