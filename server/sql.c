#include "server.h"
#include "package.h"

static int max_user_id = 99999;

void is_sqlite_ok(int ret, sqlite3 *db)
{
    if(ret == SQLITE_OK)
	{
	    //printf("sqlite %d succse \n",__LINE__);
	}
	else
	{
		printf("sqlite3_exec11: %s\n", sqlite3_errmsg(db));
	    //exit(1);
	}
}


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
    is_sqlite_ok(ret, ppdb);

    char sql[1024] = {0};
    sprintf(sql, "select max(id) from user;");
    ret = sqlite3_exec(ppdb, sql, find_max_id, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    max_user_id++;

    sprintf(sql, "insert into user values ('%d', '%s', '%s');", max_user_id, Pmsg->name, Pmsg->passwd);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    Pmsg->id = max_user_id;
    Pmsg->revert = RETURNID;

    sqlite3_close(ppdb);

    ret = sqlite3_open("friend_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "create table if not exists '%s' (id unsigned int primary key, name text, flag int);", Pmsg->name);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

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
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "select * from user where id = '%d' and passwd = '%s';", Pmsg->id, Pmsg->passwd);
    ret = sqlite3_exec(ppdb, sql, login_callback, &i_condition_login, NULL);
    is_sqlite_ok(ret, ppdb);

    ret = sqlite3_exec(ppdb, sql, login_callback_name, &(Pmsg->name), NULL);
    is_sqlite_ok(ret, ppdb);

    if(i_condition_login == 1)
    {
        Pmsg->revert = LOGINOK;
    }
    else
    {
        Pmsg->revert = LOGINFAIL;
    }

    sqlite3_close(ppdb);
}

void updata_passwd(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[512] = {0};
    char passwd[PSWSIZE];

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    int id;
    id = Pmsg->id;
    strncpy(passwd, Pmsg->passwd, PSWSIZE);

    sprintf(sql, "update user set passwd = '%s' where id = '%d';", passwd, id);  
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}


void find_friend(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[512] = {0};
    char passwd[PSWSIZE];

    ret = sqlite3_open("friend_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "select * from '%s' where id = %d;", Pmsg->name, Pmsg->toid);

    sqlite3_stmt *stmt = NULL;
    ret = sqlite3_prepare_v2(ppdb, sql, strlen(sql), &stmt, NULL);
    is_sqlite_ok(ret, ppdb);

    ret = sqlite3_step(stmt);
    if (ret == SQLITE_DONE) 
    {
        sqlite3_finalize(stmt);
        sqlite3_close(ppdb);

        Pmsg->revert = NOEXIST;
        return;
    }
    else if(ret == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(ppdb);

        Pmsg->revert = EXIST;
        return;
    }
}





static int toid_to_toname_callback(void *para, int columnCount, char **columnValue, char **columnName)
{
    strcpy((char *)para, columnValue[0]);

    return 0;
}


void toid_to_toname(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[512] = {0};
    char passwd[PSWSIZE];

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "select name from user where id = %d;", Pmsg->toid);  
    ret = sqlite3_exec(ppdb, sql, toid_to_toname_callback, &(Pmsg->toname), NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}


void add_friend_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[512] = {0};

    ret = sqlite3_open("friend_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "insert into '%s' (id, name) values ('%d', '%s');", Pmsg->name, Pmsg->toid, Pmsg->toname);//向自己好友表中插入对方id和用户名
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "insert into '%s' (id, name) values ('%d', '%s');", Pmsg->toname, Pmsg->id, Pmsg->name);//向对方好友表中插入自己id和用户名
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}