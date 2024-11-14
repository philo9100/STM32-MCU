#include "Inf_W5500_HTTPServer.h"

uint8_t txBuff[2048];
uint8_t rxBuff[2048];
uint8_t cnt = 8;
uint8_t socklist[] = {1, 2, 3, 4, 5, 6, 7, 8};

uint8_t *contentName = "index.html";
const uint8_t *content = "Hello World";

void Inf_W5500_HTTPServerInit(void)
{
    /* 1.初始化HTTPServer

        参数3：http服务器需要使用的socket的个数
        参数4：用到的socket索引
    */
    httpServer_init(txBuff, rxBuff, cnt, socklist);

    /* 2.注册httpserver的内容 */
    reg_httpServer_webContent(contentName, content);
}

void Inf_W5500_HTTPServerStart(void)
{
    /* 启动服务器 */
    httpServer_run(0);
}

uint8_t Inf_W5500_ParseAction(uint8_t *url)
{
    uint8_t *actionIndex;
    actionIndex = strstr(url, "action=");
    if (actionIndex == NULL)
    {
        return "0";
    }
    else
    {
        return (uint8_t)(*(actionIndex + 7));
    }
}

void Inf_W5500_DoAction(uint8_t *url)
{
    /* 处理http请求 */
    /*
    index.html?action=1
    index.html?action=2
    index.html?
    */
    uint8_t action = Inf_W5500_ParseAction(url);
    printf("action = %c\r\n", action);

    if (action == '1')
    {
        Driver_LED_ON(LED_2);
    }
    else if (action == '2')
    {
        Driver_LED_OFF(LED_2);
    }
    else
    {
        
    }
}
