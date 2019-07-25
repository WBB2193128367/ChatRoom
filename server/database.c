#include "server.h"
#include "package.h"


void InitDataBase(void)
{
    #if 1
    sqlite3 *ppdb; //数据库句柄

    int ret = sqlite3_open("user_info.db", &ppdb);
    if (ret != SQLITE_OK)
    {
        //printf("sqlite3_open : %s\n", sqlite3_errmsg(ppdb));
	    exit(1);
    }

    char sql[1024] = {0}; //保存数据库语句
    sprintf(sql, "create table if not exists user (id unsigned int primary key, name text, passwd text);");
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec11: %s\n", sqlite3_errmsg(ppdb));
	    exit(1);
    }

    sprintf(sql, "create table if not exists chat_history (name text, id int, toname text, toid int, msg text, flag int, group_id int, group_name text);");
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec11: %s\n", sqlite3_errmsg(ppdb));
	    exit(1);
    }
    sqlite3_close(ppdb);

    ret = sqlite3_open("group_info.db", &ppdb); 
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "create table if not exists group_list (id unsigned int primary key, name text);");
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
#if 0
    sprintf(sql, "insert into user values ('%d', '%s', '%s');", 10000, "admin", "123456");
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    if (ret != SQLITE_OK)
    {
        printf("sqlite3_exec11: %s\n", sqlite3_errmsg(ppdb));
	    exit(1);
    }
#endif
    sqlite3_close(ppdb);
    #endif
}