#include "client.h"
#include "package.h"

void process_chat(int fd, char * name)
{
    Msg msg;

    printf("启动私聊功能\n");

    printf("请输入想要发送的人的名字\n");
    scanf("%s", msg.toname);

    printf("请输入想要发送的信息\n");
    scanf("%s", msg.msg);

    strncpy(msg.name, name, NAMESIZE);
    msg.cmd = CHATTO;

    mysend(fd, &msg);
}


void process_showOnline(int fd)
{
    
    Msg msg;
    int ret;

    msg.cmd = SHOWONLINE;

    mysend(fd, &msg);
#if 0
    
#endif
}