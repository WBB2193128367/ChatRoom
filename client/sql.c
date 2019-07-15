#include "client.h"
#include "package.h"


void is_sqlite(int ret)  //测试数据库
{
    if(ret == SQLITE_OK)
	{
        printf("sqlite %d succse \n",__LINE__);
	}
	else
	{
		printf("数据库发生错误，请使用SQLITE3 看数据库!\n");
     	printf("sqlite %d error\n",__LINE__);
		exit(1);
	}
}

void is_sqlite_ok(int ret, sqlite3 *db)
{
    if(ret == SQLITE_OK)
	{
	    printf("sqlite %d succse \n",__LINE__);
	}
	else
	{
		printf("sqlite3_exec11: %s\n", sqlite3_errmsg(db));
	    exit(1);
	}
}


void open_db(sqlite3 **db)
{
    int ret;
    ret = sqlite3_open("client.db", db);
    is_sqlite(ret);
}

void create_user_db(sqlite3 *db, char *name)
{
    int ret;
    char sql[128];
    sprintf(sql, "create table if not exists '%s' (id unsigned int primary key, name text, online_flag int);", name);
    ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, db);
}


void insert_retfriend_db(sqlite3 *db, Msg *Pmsg)
{
	int ret;
	char sql[1024];

	sprintf(sql, "insert into %s values('%d', '%s');", Pmsg->name, Pmsg->toid, Pmsg->toname);
	ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
	is_sqlite_ok(ret, db);
}


void insert_addfriend_db(sqlite3 *db, Msg *Pmsg)
{
	int ret;
	char sql[1024];

	sprintf(sql, "insert into %s values('%d', '%s');", Pmsg->toname, Pmsg->id, Pmsg->name);
	ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
	is_sqlite_ok(ret, db);
}