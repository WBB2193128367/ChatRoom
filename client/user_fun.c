#include "client.h"
#include "package.h"



void login(char *name, int fd)
{
    Msg msg;
    int ret;
    char flag = 'y';

    while(flag == 'y' || flag == 'Y')
    {
        printf("请输入您的ID：\n");
        scanf("%d", &msg.id);
/*
        if(strlen(msg.msg) > NAMESIZE)
        {
            printf("您输入的用户名超过了%d个字符, 请重新输入\n", NAMESIZE);
            continue;
        }
*/
        msg.cmd = LOGIN;

        while(1)
        {
            printf("请输入密码(不要超过%d个字符)：\n", PSWSIZE);
            scanf("%s", msg.passwd);

            if(strlen(msg.passwd) > PSWSIZE)
            {
                printf("您输入的密码超过了%d个字符, 请重新输入\n", PSWSIZE);
                continue;
            }
            break;
        }

        mysend(fd, &msg);

        myrecv(fd, &msg);

        if(msg.revert == LOGINOK)
        {
            system("clear");
            printf("登陆成功！\n");
            strncpy(name, msg.name, NAMESIZE);
        }
        else if(msg.revert == LOGINFAIL)
        {
            system("clear");
            printf("登录失败!请重试！\n");
        }
        else if(msg.revert == ONLINEIN)
        {
            system("clear");
            printf("您以在其它地方登陆，你的账号可能被盗用，请尽快修改密码！\n");
        }

        sleep(2);

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
            scanf("%s", msg.name);

            if(strlen(msg.name) > NAMESIZE)
            {
                printf("您输入的用户名超过了%d个字符, 请重新输入\n", NAMESIZE);
                continue;
            }

            msg.cmd = REG;

            printf("请输入密码(不要超过%d个字符)：\n", PSWSIZE);
            scanf("%s", msg.passwd);

            if(strlen(msg.passwd) > PSWSIZE)
            {
                printf("您输入的密码超过了%d个字符, 请重新输入\n", PSWSIZE);
                continue;
            }

            mysend(fd, &msg);

            break;
        }

        printf("是否继续申请注册，(Y/N)\n");
        getchar();
        scanf("%c", &flag);
    }

    myrecv(fd, &msg);

    if(msg.revert == RETURNID)
        printf("您可用于登陆的id为：%d\n", msg.id);
}



void logout(int fd)
{
    int ret;
    Msg msg;

    msg.cmd = LOGOUT;

    mysend(fd, &msg);

    printf("退出成功\n");
}



void exit_client(int fd)
{
    int ret;
    Msg msg;

    msg.cmd = EXIT;

    mysend(fd, &msg);

    shutdown(fd, SHUT_WR);

    close(fd);

    printf("退出成功\n");

    exit(0);
}



