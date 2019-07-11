#include "server.h"
#include "package.h"

static int max_user_id = 99999;
static int find_max_id(void *para, int columnCount, char **columnValue, char **columnName)
{

    debug_msg("%s : %d\n", __FILE__, __LINE__);
    printf("%s\n", columnValue[0]);
    max_user_id = atoi(columnValue[0]);
    

    return 0;
}


int reg_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;

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

    max_user_id++;

    sprintf(sql, "insert into user values ('%d', '%s', '%s');", max_user_id, Pmsg->name, Pmsg->passwd);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    if(ret != SQLITE_OK)
    {
        printf("insert fail : %s\n", sqlite3_errmsg(ppdb));
    }

    Pmsg->id = max_user_id;
    Pmsg->revert = RETURNID;

    sqlite3_close(ppdb);

    return max_user_id;
}





static int login_callback(void *para, int columnCount, char **columnValue, char **columnName)
{
    *(int *)para = 1;

    return 0;
}


static int login_callback_name(void *para, int columnCount, char **columnValue, char **columnName)
{
    strcpy((char *)para, columnValue[1]);

    return 0;
}

void log_db(Msg * Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};
    int i_condition_login = 0;

    ret = sqlite3_open("user_info.db", &ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open error : %s\n", sqlite3_errmsg(ppdb));
    }

    sprintf(sql, "select * from user where id = '%d' and passwd = '%s';", Pmsg->id, Pmsg->passwd);
    ret = sqlite3_exec(ppdb, sql, login_callback, &i_condition_login, NULL);
    if(ret != SQLITE_OK)
    {
        printf("select fail : %s\n", sqlite3_errmsg(ppdb));
    }

    ret = sqlite3_exec(ppdb, sql, login_callback_name, &(Pmsg->name), NULL);
    if(ret != SQLITE_OK)
    {
        printf("select fail : %s\n", sqlite3_errmsg(ppdb));
    }

    if(i_condition_login == 1)
    {
        Pmsg->revert = LOGINOK;
    }
    else
    {
        Pmsg->cmd = LOGINFAIL;
    }
}