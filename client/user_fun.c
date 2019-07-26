#include "client.h"
#include "package.h"

Link head;

/*用户ID登录 */
void login(char *name, int fd)
{
    extern int id_global;
    Msg msg;
    int ret;
    msg.cmd = LOGIN;

    insert_id();
    scanf("%d", &msg.id);
    getchar();
    printf("\033[60C");
    char *passwd;
    passwd = (char *)get_passwd();
    printf("\n");
    strncpy(msg.passwd, passwd, PSWSIZE);

    mysend(fd, &msg);

    printf("\n");
    printf("\033[50C");
    printf("正在登录，请稍等！\n");

    myrecv(fd, &msg);

    if (msg.revert == LOGINOK)
    {
        sleep(1);
        printf("\033[2A");
        printf("\n");
        printf("\033[50C");
        printf("您已登录成功,欢迎您的使用！\n");
        strncpy(name, msg.name, NAMESIZE);
        id_global = msg.id;
    }
    else if (msg.revert == LOGINFAIL)
    {
        //system("clear");
        //printf("登录失败!请重试！\n");
        sleep(1);
        printf("\033[2A");
        printf("\n");
        printf("\033[45C");
        printf("您输入的密码有误或者用户不存在！\n");
    }
    else if (msg.revert == ONLINEIN)
    {
        //system("clear");
        //printf("您以在其它地方登陆，你的账号可能被盗用，请尽快修改密码！\n");
        sleep(1);
        printf("\033[2A");
        printf("\n");
        printf("\033[50C");
        printf("该用户已经登陆！\n");
    }
}

/*用户注册 */
void user_reg(int fd)
{
    Msg msg;
    int ret;
#if 0
    printf("请输入用户名(不要超过%d个字符)：\n", NAMESIZE);
    scanf("%s", msg.name);
/*
    if(strlen(msg.name) > NAMESIZE)
    {
        printf("您输入的用户名超过了%d个字符, 请重新输入\n", NAMESIZE);
    }
*/
    msg.cmd = REG;

    printf("请输入密码(不要超过%d个字符)：\n", PSWSIZE);
    scanf("%s", msg.passwd);
/*
    if(strlen(msg.passwd) > PSWSIZE)
    {
        printf("您输入的密码超过了%d个字符, 请重新输入\n", PSWSIZE);
    }
*/
#endif
    insert_reg();
    char *passwd;
    char *passwd1;
    scanf("%s", msg.name);
    getchar();
    printf("\033[60C");
    passwd = get_passwd();
    printf("\n");
    printf("\033[60C");
    passwd1 = get_passwd();
    msg.cmd = REG;

    if (strcmp(passwd, passwd1) != 0)
    {
        printf("\n");
        printf("\033[50C");
        printf("您输入的密码不一致！");
        printf("\033[2B");
        printf("\n");
        return;
    }
    strncpy(msg.passwd, passwd, PSWSIZE);
    printf("\n");
    printf("\033[50C");
    printf("正在注册，请稍等！\n");

    mysend(fd, &msg);

    myrecv(fd, &msg);

    if (msg.revert == RETURNID)
    {
        //printf("您可用于登陆的id为：%d\n", msg.id);
        sleep(1);
        printf("\033[2A");
        printf("\n");
        printf("\033[50C");
        printf("您已注册成功,欢迎您的使用！\n");
        printf("\033[50C");
        printf("请记住你的ID号：%d",msg.id);
        printf("\033[2B");
        printf("\n");
    }
    else if(msg.revert == EXIST)
    {
        sleep(1);
        printf("\033[2A");
        printf("\n");
        printf("\033[50C");
        printf("已有账户注册过此用户名！");
        printf("\033[3B");
        printf("\n");
    }
}

/*用户用户名登录 */
void login_name(char *name, int fd)
{
    extern int id_global;
    Msg msg;
    int ret;
    msg.cmd = LOGINNAME;

    insert_log();
	scanf("%s",msg.name);
	getchar();
	printf("\033[60C"); 
    char *passwd;
	passwd =(char *)get_passwd();
	printf("\n");
	strncpy(msg.passwd, passwd, PSWSIZE);

    mysend(fd,&msg);
    printf("\n");
	printf("\033[50C");
    printf("正在登录，请稍等！\n");

    myrecv(fd, &msg);

    if (msg.revert == LOGINOK)
    {
        sleep(1);
        printf("\033[2A");
        printf("\n");
        printf("\033[50C");
        printf("您已登录成功,欢迎您的使用！\n");
        strncpy(name, msg.name, NAMESIZE);
        id_global = msg.id;
    }
    else if (msg.revert == LOGINFAIL)
    {
        //system("clear");
        //printf("登录失败!请重试！\n");
        sleep(1);
        printf("\033[2A");
        printf("\n");
        printf("\033[45C");
        printf("您输入的密码有误或者用户不存在！\n");
    }
    else if (msg.revert == ONLINEIN)
    {
        //system("clear");
        //printf("您以在其它地方登陆，你的账号可能被盗用，请尽快修改密码！\n");
        sleep(1);
        printf("\033[2A");
        printf("\n");
        printf("\033[50C");
        printf("该用户已经登陆！\n");
    }
}


void logout(int fd, char *name)
{
    extern int id_global;
    int ret;
    Msg msg;

    msg.cmd = LOGOUT;
    strncpy(msg.name, name, NAMESIZE);
    msg.id = id_global;

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
