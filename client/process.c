#include "client.h"
#include "package.h"

extern sqlite3 *db;
extern int id_global;
extern char name_global[NAMESIZE];

void process_chat(int fd, char * name)
{
    Msg msg;

    printf("启动私聊功能\n");

    printf("请输入想要发送的人的名字\n");
    scanf("%s", msg.toname);

    printf("请输入想要发送的信息\n");
    scanf("%s", msg.msg);

    strncpy(msg.name, name, NAMESIZE);
    msg.id = id_global;
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


void process_chattoid(int fd, char *name)
{
    Msg msg;
    extern Link head;
    Link p;

    printf("启动私聊功能\n");

    printf("请输入想要发送的人的id\n");
    scanf("%d", &msg.toid);

    printf("请输入想要发送的信息\n");
    scanf("%s", msg.msg);

    strncpy(msg.name, name, NAMESIZE);
    msg.id = id_global;
    /* while(p != NULL)
    {
        if(p->id == msg.toid)
        {
            strncpy(msg.toname, p->name, NAMESIZE);
            break;
        }
        p = p->next;
    } */
    msg.cmd = CHATTOID;

    mysend(fd, &msg);
}


void process_get_offline(int fd)
{
    Msg msg;
    msg.cmd = GETOFFLINE;
    msg.id = id_global;

    mysend(fd, &msg);
}


void process_showGroup(int fd)
{
    Msg msg;
    msg.cmd = SHOWGROUP;
    msg.id = id_global;

    mysend(fd, &msg);
}


void process_joinGroup(int fd)
{
    Msg msg;
    msg.cmd = JOINGROUP;
    printf("请输入想要加入的群的ID\n");
    scanf("%d", &msg.group_id);
    msg.id = id_global;
    strcpy(msg.name, name_global);

    mysend(fd, &msg);
}


void process_creatGroup(int fd)
{
    Msg msg;
    msg.cmd = CREATGROUP;
    msg.id = id_global;
    strcpy(msg.name, name_global);
    
    printf("请输入群名称\n");
    scanf("%s", msg.group);

    mysend(fd, &msg);
}


/*发送群消息 */
void process_chatGroup(int fd)
{
    Msg msg;

    msg.cmd = CHATGROUP;
    printf("请输入群ID:\n");
    scanf("%d", &msg.group_id);

    printf("请输入发送的信息:\n");
    scanf("%s", msg.msg);

    msg.id = id_global;
    strcpy(msg.name, name_global);
    debug_msg("name_global = %s\n", name_global);
    debug_msg("Pmsg->name = %s\n", msg.name);

    mysend(fd, &msg);
}