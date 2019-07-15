#include "client.h"
#include "package.h"

extern sqlite3 *db;
extern int id_global;

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
    debug_msg("show online\n");
    Msg msg;
    int ret;

    msg.cmd = SHOWONLINE;

    mysend(fd, &msg);
}


void process_addFriend(int fd, char *name)
{
    Msg msg;
    int ret;

    printf("请输入对方的id\n");
    scanf("%d", &msg.toid);

    msg.id = id_global;

    strncpy(msg.name, name, NAMESIZE);
    msg.cmd = ADDFRIEND;

    mysend(fd, &msg);
}


void process_retFriend(int fd, char *name)
{
    Msg msg;
    int ret;

    printf("请输入对方的id\n");
    scanf("%d", &msg.toid);

    msg.id = id_global;
    strncpy(msg.name, name, NAMESIZE);

    //添加到数据库

    msg.cmd = RETFRIEND;

    mysend(fd, &msg);
}


void process_passwd(int fd)
{
    char passwd[PSWSIZE];
    char passwd2[PSWSIZE];
    Msg msg;

    printf("请输入新密码\n");
    scanf("%s", passwd);

    printf("请再次输入密码\n");
    scanf("%s", passwd2);

    if(strcmp(passwd, passwd2) == 0)
    {
        strcpy(msg.passwd, passwd);
        msg.cmd = PASSWD;
        msg.id = id_global;
        mysend(fd, &msg);
    }
    else
    {
        printf("您两次输入的密码不一样，请重新修改！\n");
    }    
}


void process_chatall(int fd, char *name)
{
    Msg msg;

    printf("请输入您想给所有人发送的消息\n");
    scanf("%s", msg.msg);

    strncpy(msg.name, name, NAMESIZE);
    msg.cmd = CHATALL;

    mysend(fd, &msg);
}