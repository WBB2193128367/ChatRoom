#include "client.h"



int login(int fd)
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
        if(ret < 0)
        {
            perror("send error");
            exit(1);
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
            if(ret < 0)
            {
                perror("send error");
                exit(1);
            }

            break;
        }

        ret = recv(fd, &msg, sizeof(msg), 0);
        if(ret < 0)
        {
            perror("recv error");
        }

        if(msg.cmd == LOGINOK)
        {
            printf("登陆成功！\n");
            return LOGINOK;
        }
        else if(msg.cmd == LOGINFAIL)
        {
            printf("登录失败!请重试！\n");
            return LOGINFAIL;
        }
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
            if(ret < 0)
            {
                perror("send error");
                exit(1);
            }

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
            if(ret < 0)
            {
                perror("send error");
                exit(1);
            }

            break;
        }

        printf("是否继续申请注册，(Y/N)\n");
        getchar();
        scanf("%c", &flag);
    }

    ret = recv(fd, &msg, sizeof(msg), 0);
    if(ret < 0)
    {
        perror("recv error");
        exit(1);
    }

    if(msg.cmd == RETURNID)
        printf("您可用于登陆的id为：%s\n", msg.msg);
}