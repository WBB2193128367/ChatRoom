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