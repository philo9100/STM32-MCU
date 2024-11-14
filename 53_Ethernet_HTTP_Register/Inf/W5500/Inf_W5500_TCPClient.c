#include "Inf_W5500_TCPClient.h"

uint8_t listenStatus;
uint8_t dip[4] = {192, 168, 31, 228};
uint16_t dport = 8888;

uint8_t cnctStatus;

uint8_t rxBuff[2048] = {0};
uint16_t rxLen = 0;

void Inf_W5500_TCPClient(void)
{
    /* 1.先获取socket的状态 参数：要获取的socket的索引 0-7 */
    uint8_t socketStatus = getSn_CR(0);

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
        uint8_t sn = socket(0, Sn_MR_TCP, 8080, SF_TCP_NODELAY);
        if (sn == 0)
        {
            printf("socket 0 打开成功");
        }
        else
        {
            printf("socket 0 打开失败");
        }
        break;

    case SOCK_INIT: /* 表示socket已经打开了 可以作为服务端监听 也可以作为客户端连接其他服务器 */
        /* 作为客户端去主动连接服务器 */
        cnctStatus = connect(0, dip, dport);
        if (cnctStatus == SOCK_OK)
        {
            printf("客户端连接成功");
        }
        else
        {
            close(0);
            printf("客户端连接失败");
        }
        break;
    case SOCK_ESTABLISHED: /* 表示socket连接建立成功 */
        
        send(0, "hello this is stm32 tcp client\r\n", 33);
        
        getSn_DIPR(0, dip);
        dport = getSn_DPORT(0);
        printf("客户端ip=%d.%d.%d.%d,端口=%d", dip[0], dip[1], dip[2], dip[3], dport);
        while (1)
        {
            /* 正常通讯 */
            /* 等待客户端发送信息 通过中断寄存器的 REVE位来判断 */
            while ((getSn_IR(0) & Sn_IR_RECV) == 0)
            {
                /* 在等待客户端发送信息的时候，客户端有可能断开连接（连接发生断开） */
                if (getSn_SR(0) != SOCK_ESTABLISHED)
                {
                    printf("客户端已断开连接");
                    close(0);
                    return;
                }
            }

            /* 收到信息了，清除接收中断标志位 写1表示清除 写0无效 */
            setSn_IR(0, Sn_IR_RECV);

            /* 获取收到的数据的字节数 */
            rxLen = getSn_RX_RSR(0);
            if (rxLen > 0)
            {
                /* 接收数据 */
                recv(0, rxBuff, rxLen);
                printf("%d.%d.%d.%d:%d 发来的数据：%.*s\r\n", dip[0], dip[1], dip[2], dip[3], dport, rxLen, rxBuff);
                /* 回显：把数据再发回去 */
                send(0, rxBuff, rxLen);
            }
        }

        break;
    default:
        break;
    }
}