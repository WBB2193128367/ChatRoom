#include "client.h"



void login(char *name, int fd)
{
    Msg msg;
    int ret;
    char flag = 'y';

    while(flag == 'y' || flag == 'Y')
    {
        printf("请输入您的ID：\n");
        scanf("%s", msg.msg);

        if(strlen(msg.msg) > NAMESIZE)
        {
            printf("您输入的用户名超过了%d个字符, 请重新输入\n", NAMESIZE);
            continue;
        }

        msg.cmd = LOGIN;

        ret = send(fd, &msg, sizeof(msg), 0);
        is_send_recv_ok(ret, "send error");

        while(1)
        {
            printf("请输入密码(不要超过%d个字符)：\n", PSWSIZE);
            scanf("%s", msg.msg);

            if(strlen(msg.msg) > PSWSIZE)
            {
                printf("您输入的密码超过了%d个字符, 请重新输入\n", PSWSIZE);
                continue;
            }

            ret = send(fd, &msg, sizeof(msg), 0);
            is_send_recv_ok(ret, "send error");

            break;
        }

        ret = recv(fd, &msg, sizeof(msg), 0);
        is_send_recv_ok(ret, "recv error");

        if(msg.cmd == LOGINOK)
        {
            printf("登陆成功！\n");
            strncpy(name, msg.msg, NAMESIZE);
        }
        else if(msg.cmd == LOGINFAIL)
        {
            printf("登录失败!请重试！\n");
        }

        break;
    }
}



void user_reg(int fd)
{
    Msg msg;
    int ret;
    char flag = 'y';

    while(flag == 'y' || flag == 'Y')
    {
        while(1)
        {
            printf("请输入用户名(不要超过%d个字符)：\n", NAMESIZE);
            scanf("%s", msg.msg);

            if(strlen(msg.msg) > NAMESIZE)
            {
                printf("您输入的用户名超过了%d个字符, 请重新输入\n", NAMESIZE);
                continue;
            }

            msg.cmd = REG;

            ret = send(fd, &msg, sizeof(msg), 0);
            is_send_recv_ok(ret, "send error");

            break;
        }

        while(1)
        {
            printf("请输入密码(不要超过%d个字符)：\n", PSWSIZE);
            scanf("%s", msg.msg);

            if(strlen(msg.msg) > PSWSIZE)
            {
                printf("您输入的密码超过了%d个字符, 请重新输入\n", PSWSIZE);
                continue;
            }

            ret = send(fd, &msg, sizeof(msg), 0);
            is_send_recv_ok(ret, "send error");

            break;
        }

        printf("是否继续申请注册，(Y/N)\n");
        getchar();
        scanf("%c", &flag);
    }

    ret = recv(fd, &msg, sizeof(msg), 0);
    is_send_recv_ok(ret, "recv error");

    if(msg.cmd == RETURNID)
        printf("您可用于登陆的id为：%s\n", msg.msg);
}



void logout(int fd)
{
    int ret;
    Msg msg;

    msg.cmd = LOGOUT;

    ret = send(fd, &msg, sizeof(msg), 0);
    is_send_recv_ok(ret, "send error");

    printf("退出成功\n");
}



void exit_client(int fd)
{
    int ret;
    Msg msg;

    msg.cmd = EXIT;

    ret = send(fd, &msg, sizeof(msg), 0);
    is_send_recv_ok(ret, "send error");

    shutdown(fd, SHUT_WR);

    close(fd);

    printf("退出成功\n");

    exit(0);
}



void showOnlineFriend(int fd)
{
    #if 0
    Msg msg;
    int ret;

    msg.cmd = SHOWONLINE;

    ret = send(fd, &msg, sizeof(msg), 0);
    is_send_recv_ok(ret, "send error");

    Link p, head;
    p = head = NULL;
    Link newnode;

    while(1)
    {
        create_node(&newnode);
        ret = recv(fd, newnode, sizeof(Node), 0);
        is_send_recv_ok(ret, "recv error");

        if(newnode->id == 0)
        {
            break;
        }
        else if(newnode->id != 0)
        {
            if(head == NULL)
            {
                head = newnode;
                head->next = NULL;
                p = head;
            }
            else
            {
                (p->next) = newnode;
                newnode->next = NULL;
            }
        }    
    }

    display_list(head);
    #endif
}