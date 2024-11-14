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
    printf("�й�� I2C Ӳ�� ʵ�鿪ʼ....\r\n");
    Driver_LED_Init();
    Driver_Key_Init();
    Inf_W24C02_Init();

    /* 1. �ȴ�E2PROM�ж�ȡָ�� */
    Inf_W24C02_ReadBytes(E2PROM_START_ADDR, buff, CMD_LEN);
    /* 2. ���û�ж�ȡ��ָ��, �ȴ������·�ָ�� */
    if (strstr((char *)buff, "#") == NULL)
    {
        printf("��E2PROM��û�ж���ָ��...\r\n");
        /* 2.1 �ȴ������·�ָ�� */
        printf("�ȴ������·�ָ��...\r\n");
        while (!isToSend)
            ;
        isToSend = 0;
        printf("�·���ָ����: %s\r\n", buff);
        /* 2.3 ��ָ���ַ���д�뵽E2PROM */
        Inf_W24C02_WriteBytes(E2PROM_START_ADDR, buff, CMD_LEN);
    }
    /* 3. ��ȡ����ָ�� ����ָ��. �ѽ������������ִ洢��һ����ά�������� */
    printf("��ʼ����ָ��... %s \r\n", buff);
    handleCMD();
    while (1)
    {
        // 4. ��ˮ����ʾ
        doLedFlow();
        // 5. ��ⰴ��
        checkKey();
        // 6. ��⴮��
        checkUsart();
    }
}

// ��������
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
        /* �ȹر����е�LED�� */
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
        printf("�㰴���˰���: %d\r\n", index);
    }
}

void checkUsart(void)
{
    if (isToSend)
    {
        printf("�·����µ�����: %s\r\n", buff);
        isToSend = 0;
        handleCMD();
        Inf_W24C02_WriteBytes(E2PROM_START_ADDR, buff, CMD_LEN);
    }
}