#include "client.h"
#include "package.h"

void is_sqlite(int ret) //测试数据库
{
	if (ret == SQLITE_OK)
	{
		//printf("sqlite %d succse \n",__LINE__);
	}
	else
	{
		printf("数据库发生错误，请使用SQLITE3 看数据库!\n");
		printf("sqlite %d error\n", __LINE__);
		exit(1);
	}
}

void is_sqlite_ok(int ret, sqlite3 *db)
{
	if (ret == SQLITE_OK)
	{
		//printf("sqlite %d succse \n",__LINE__);
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
	extern char name_global[NAMESIZE];
	char db_file_name[NAMESIZE + 20];
	sprintf(db_file_name, "./user_db/%s.db", name_global);
	ret = sqlite3_open(db_file_name, db);
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

void history_msg_person_db(sqlite3 *db, Msg *Pmsg)
{
	int ret;
	char sql[2048];

	if (Pmsg->revert == CHATOK)
	{
		sprintf(sql, "create table if not exists chat_history_%s (id int, name text, toid int, toname text, history_msg text);", Pmsg->toname);
		ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		is_sqlite_ok(ret, db);

		sprintf(sql, "insert into chat_history_%s values ('%d', '%s', '%d', '%s', '%s');", Pmsg->toname, Pmsg->id, Pmsg->name, Pmsg->toid, Pmsg->toname, Pmsg->msg);
		ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		is_sqlite_ok(ret, db);
	}
	else if(Pmsg->revert == CHATTO || Pmsg->revert == OFFLINE)
	{
		sprintf(sql,"create table if not exists chat_history_%s (id int, name text, toid int, toname text, history_msg text);", Pmsg->name);
		ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		is_sqlite_ok(ret, db);

		sprintf(sql, "insert into chat_history_%s values ('%d', '%s', '%d', '%s', '%s');", Pmsg->name, Pmsg->id, Pmsg->name, Pmsg->toid, Pmsg->toname, Pmsg->msg);
		ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
		is_sqlite_ok(ret, db);
	}
}

void history_msg_group_db(sqlite3 *db, Msg *Pmsg)
{
	int ret;
	char sql[2048];

	sprintf(sql, "create table if not exists chat_history_%s (id int, name text, group_id int, group_name text, history_msg text);", Pmsg->group);
	ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
	is_sqlite_ok(ret, db);

	sprintf(sql, "insert into chat_history_%s values ('%d', '%s', '%d', '%s', '%s');", Pmsg->group, Pmsg->id, Pmsg->name, Pmsg->group_id, Pmsg->group, Pmsg->msg);
	ret = sqlite3_exec(db, sql, NULL, NULL, NULL);
	is_sqlite_ok(ret, db);
}