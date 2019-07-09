#include "server.h"



static int max_user_id = 99999;


static int find_max_id(void *para, int columnCount, char **columnValue, char **columnName)
{

    debug_msg("%s : %d\n", __FILE__, __LINE__);
    printf("%s\n", columnValue[0]);
    max_user_id = atoi(columnValue[0]);
    

    return 0;
}


void user_reg(Msg *Pmsg, int fd)
{
    Msg msg = *(Pmsg);
    sqlite3 *ppdb;
    char name[NAMESIZE];
    char psw[PSWSIZE];
    int ret;

    strncpy(name, msg.msg, NAMESIZE);

    ret = sqlite3_open("user_info.db", &ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open error : %s\n", sqlite3_errmsg(ppdb));
    }

    char sql[1024] = {0};
    sprintf(sql, "select max(id) from user;");
    ret = sqlite3_exec(ppdb, sql, find_max_id, NULL, NULL);
    if(ret != SQLITE_OK)
    {
        printf("find_max_id fail : %s\n", sqlite3_errmsg(ppdb));
    }

    ret = recv(fd, &msg, sizeof(msg), 0);
    if(ret < 0)
    {
        perror("recv error");
    }

    strncpy(psw, msg.msg, PSWSIZE);

    max_user_id++;

    sprintf(sql, "insert into user values ('%d', '%s', '%s');", max_user_id, name, psw);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    if(ret != SQLITE_OK)
    {
        printf("insert fail : %s\n", sqlite3_errmsg(ppdb));
    }

    memset(&msg, 0, sizeof(msg));

    sprintf(msg.msg, "%d", max_user_id);
    msg.cmd = RETURNID;

    ret = send(fd, &msg, sizeof(msg), 0);
    if(ret < 0)
    {
        perror("send error");
    }
}



static int login_callback(void *para, int columnCount, char **columnValue, char **columnName)
{
    *(int *)para = 1;  

    return 0;
}


int login(Msg * Pmsg, int fd)
{
    Msg msg = *(Pmsg);
    int id;
    char sql[1024] = {0};

    sqlite3 *ppdb;
    char psw[PSWSIZE];
    int ret;
    int i_condition_login = 0;

    id = atoi(msg.msg);

    ret = sqlite3_open("user_info.db", &ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open error : %s\n", sqlite3_errmsg(ppdb));
    }

    ret = recv(fd, &msg, sizeof(msg), 0);
    if(ret < 0)
    {
        perror("recv error");
    }

    strncpy(psw, msg.msg, PSWSIZE);

    sprintf(sql, "select * from user where id = '%d' and passwd = '%s';", id,psw);
    ret = sqlite3_exec(ppdb, sql, login_callback, &i_condition_login, NULL);
    if(ret != SQLITE_OK)
    {
        printf("select fail : %s\n", sqlite3_errmsg(ppdb));
    }

    if(i_condition_login == 1)
    {
        msg.cmd = LOGINOK;
        ret = send(fd, &msg, sizeof(msg), 0);
        if(ret < 0)
        {
            perror("send error");
        }

        return LOGINOK;
    }
    else
    {
        msg.cmd = LOGINFAIL;
        ret = send(fd, &msg, sizeof(msg), 0);
        if(ret < 0)
        {
            perror("send error");
        }

        return LOGINFAIL;
    }
    
}