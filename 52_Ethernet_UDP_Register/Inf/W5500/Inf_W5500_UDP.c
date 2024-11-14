#include "Inf_W5500_UDP.h"

uint8_t listenStatus;
uint8_t dip[4] = {192, 168, 31, 228};
uint16_t dport = 8888;

uint8_t cnctStatus;

uint8_t rxBuff[2048] = {0};
uint16_t rxLen = 0;

void Inf_W5500_UDP(void)
{
    /* 1.先获取socket的状态 参数：要获取的socket的索引 0-7 */
    uint8_t socketStatus = getSn_SR(0);

    switch (socketStatus)
    {
    case SOCK_CLOSED: /* 表示socket已经关闭  需要打开socket */
        /*
            打开一个socket
            参数一：要打开的socket的索引 0-7
            参数二：socket的协议类型 Sn_MR_TCP或Sn_MR_UDP
            参数三：socket的端口号
            参数四：socket的标志
            返回值：如果打开成功，返回socket的索引 0-7
        */
        uint8_t sn = socket(0, Sn_MR_UDP, 8080, 0);
        if (sn == 0)
        {
            printf("socket 0 打开成功");
        }
        else
        {
            printf("socket 0 打开失败");
        }
        break;
    case SOCK_UDP:

        while (1)
        {
            /* 阻塞等待是否收到数据 */
            while ((getSn_IR(0) & Sn_IR_RECV) == 0)
            {
                if (getSn_SR(0) != SOCK_UDP)
                {
                    printf("socket 0 失败\r\n");
                    close(0);
                    return;
                }
            }

            /* 收到数据后清除标志位 */
            setSn_IR(0, Sn_IR_RECV);
            /* 对UDP协议来说，收到数据的长度比实际发送的长度多8个字节 */
            rxLen = getSn_RX_RSR(0);
            if (rxLen > 0)
            {
                /* 参数4：对方的ip 参数5：对方的端口号 */
                recvfrom(0, rxBuff, rxLen-8, dip, &dport);
                printf("收到数据:%s \r\n", rxBuff);
                /* 回显：给对方原封不动的发回去 */
                sendto(0, rxBuff, rxLen-8, dip, dport);
            }
        }

        break;

    default:
        break;
    }
}