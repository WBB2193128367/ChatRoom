#include "server.h"
#include "package.h"

static int max_user_id = 9999;
static int max_group_id = 99999;

void is_sqlite_ok(int ret, sqlite3 *db)
{
    if (ret == SQLITE_OK)
    {
        //printf("sqlite %d succse \n",__LINE__);
    }
    else
    {
        printf("sqlite3_exec11: %s\n", sqlite3_errmsg(db));
        //exit(1);
    }
}


/*判断用户名是否重复 */
void is_name_exist(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    char sql[1024] = {0};
    sprintf(sql, "select * from user where name = '%s';", Pmsg->name);
    
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
    else if (ret == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(ppdb);

        Pmsg->revert = EXIST;
        return;
    }
    
}


static int find_max_user_id(void *para, int columnCount, char **columnValue, char **columnName)
{

    debug_msg("%s : %d\n", __FILE__, __LINE__);
    //printf("%s\n", columnValue[0]);
    if (columnValue[0] == NULL)
    {
        ;
    }
    else
    {
        max_user_id = atoi(columnValue[0]);
    }

    return 0;
}
/*用户注册，将用户名密码和ID写入 */
int reg_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    char sql[1024] = {0};
    sprintf(sql, "select max(id) from user;");
    ret = sqlite3_exec(ppdb, sql, find_max_user_id, NULL, NULL);
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

void log_db(Msg *Pmsg)
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

    if (i_condition_login == 1)
    {
        Pmsg->revert = LOGINOK;
    }
    else
    {
        Pmsg->revert = LOGINFAIL;
    }

    sqlite3_close(ppdb);
}


static int login_callback_id(void *para, int columnCount, char **columnValue, char **columnName)
{
    //strcpy((char *)para, columnValue[1]);
    *(int *)para = atoi(columnValue[0]);

    return 0;
}

/*用户名登录 */
void log_name_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};
    int i_condition_login = 0;

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "select * from user where name = '%s' and passwd = '%s';", Pmsg->name, Pmsg->passwd);
    ret = sqlite3_exec(ppdb, sql, login_callback, &i_condition_login, NULL);
    is_sqlite_ok(ret, ppdb);

    ret = sqlite3_exec(ppdb, sql, login_callback_id, &(Pmsg->id), NULL);
    is_sqlite_ok(ret, ppdb);

    if (i_condition_login == 1)
    {
        Pmsg->revert = LOGINOK;
    }
    else
    {
        Pmsg->revert = LOGINFAIL;
    }

    sqlite3_close(ppdb);
}

/*修改密码 */
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
    else if (ret == SQLITE_ROW)
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

    sprintf(sql, "insert into '%s' (id, name) values ('%d', '%s');", Pmsg->name, Pmsg->toid, Pmsg->toname); //向自己好友表中插入对方id和用户名
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "insert into '%s' (id, name) values ('%d', '%s');", Pmsg->toname, Pmsg->id, Pmsg->name); //向对方好友表中插入自己id和用户名
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}

/*个人离线消息 */
void offline_msg_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "insert into chat_history values ('%s', '%d', '%s', '%d', '%s', '0', '0', NULL);", Pmsg->name, Pmsg->id, Pmsg->toname, Pmsg->toid, Pmsg->msg); //向自己好友表中插入对方id和用户名
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}

/*个人消息记录 */
void online_msg_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "insert into chat_history values ('%s', '%d', '%s', '%d', '%s', '1', '0', NULL);", Pmsg->name, Pmsg->id, Pmsg->toname, Pmsg->toid, Pmsg->msg); //向自己好友表中插入对方id和用户名
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}


/*检查用户是否存在 */
void find_user_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[512] = {0};

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "select * from user where id = %d or name = '%s';", Pmsg->toid, Pmsg->toname);

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
    else if (ret == SQLITE_ROW)
    {
        Pmsg->toid = sqlite3_column_int(stmt, 0);
        const char *toname = sqlite3_column_text(stmt, 1);
        strncpy(Pmsg->toname, toname, NAMESIZE);

        sqlite3_finalize(stmt);
        sqlite3_close(ppdb);

        Pmsg->revert = EXIST;
        return;
    }
}

static int get_offline_callback(void *para, int columnCount, char **columnValue, char **columnName)
{
    int fd = *((int *)para);
    Msg msg;
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    strcpy(msg.name, columnValue[0]);
    msg.id = atoi(columnValue[1]);
    strcpy(msg.toname, columnValue[2]);
    msg.toid = atoi(columnValue[3]);
    strcpy(msg.msg, columnValue[4]);

    if (atoi(columnValue[6]) == 0)
    {
        msg.group_id = 0;
    }
    else
    {
        msg.group_id = atoi(columnValue[6]);
        strcpy(msg.group, columnValue[7]);
    }

    msg.revert = OFFLINE;
    mysend(fd, &msg);
    usleep(10);

    return 0;
}

/*获取离线消息 */
void get_offline_db(int fd, Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "select * from chat_history where toid = %d and flag = 0;", Pmsg->id);
    ret = sqlite3_exec(ppdb, sql, get_offline_callback, &fd, NULL);
    is_sqlite_ok(ret, ppdb);

    sprintf(sql, "update chat_history set flag = 1 where toid = %d and flag = 0;", Pmsg->id);
    ret = sqlite3_exec(ppdb, sql, get_offline_callback, &fd, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}

static int find_max_group_id(void *para, int columnCount, char **columnValue, char **columnName)
{

    debug_msg("%s : %d\n", __FILE__, __LINE__);
    //printf("%s\n", columnValue[0]);
    debug_msg("%s : %d\n", __FILE__, __LINE__);
    if (columnValue[0] == NULL)
    {
        ;
    }
    else
    {
        max_group_id = atoi(columnValue[0]);
    }
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    return 0;
}

void create_group_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};

    ret = sqlite3_open("group_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    /*查找目前最大的群ID号 */
    sprintf(sql, "select max(id) from group_list;");
    ret = sqlite3_exec(ppdb, sql, find_max_group_id, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    max_group_id++;

    //向总群表中添加此群的信息
    sprintf(sql, "insert into group_list values ('%d', '%s');", max_group_id, Pmsg->group);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    /* **********************
    创建此群的表，将创建人设为群主    
    flag = 0 : 非禁言
    flag = 1 : 禁言
    admin = 0 : 群成员
    admin = 1 : 管理
    admin = 2 : 群主
    ********************** */
    sprintf(sql, "create table if not exists '%d' (id unsigned int primary key, name text, flag int, admin int);", max_group_id);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    sprintf(sql, "insert into '%d' values ('%d', '%s', '0', '2');", max_group_id, Pmsg->id, Pmsg->name);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    /*向用户加入的群表中添加此群的信息 */
    sprintf(sql, "create table if not exists '%d' (id unsigned int primary key, name text);", Pmsg->id);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    sprintf(sql, "insert into '%d' values ('%d', '%s');", Pmsg->id, max_group_id, Pmsg->group);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    Pmsg->group_id = max_group_id;
    Pmsg->revert = GROUPOK;
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    sqlite3_close(ppdb);
}

static int callback_show_group(void *para, int columnCount, char **columnValue, char **columnName)
{
    Msg *Pmsg = (Msg *)para;
    Pmsg->group_id = atoi(columnValue[0]);
    strcpy(Pmsg->group, columnValue[1]);
    Pmsg->revert = SHOWGROUP;

    mysend(Pmsg->fd, Pmsg);
    usleep(10);
    return 0;
}

/*显示用户加入的群聊 */
void show_group_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};

    ret = sqlite3_open("group_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    sprintf(sql, "select * from '%d';", Pmsg->id);
    ret = sqlite3_exec(ppdb, sql, callback_show_group, Pmsg, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);
}


/*检测群ID合法的回调函数 */
static int callback_check_group_exist(void *para, int columnCount, char **columnValue, char **columnName)
{
    *(int *)para = 1;
    return 0;
}

/*
    功能：检测此群是否存在
    参数：Pmsg：group_id
    传出参数：Pmsg->revert 
                GROUPNOTEXIST：不存在
                GROUPEXIST：存在
 */
void check_group_exist(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};
    int flag = 0;
    debug_msg("Pmsg->name = %s\n", Pmsg->name);

    ret = sqlite3_open("group_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    /*检查有没有此群 */
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select * from group_list where id = '%d';", Pmsg->group_id);
    ret = sqlite3_exec(ppdb, sql, callback_check_group_exist, &flag, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    /*没有此群 */
    if (flag == 0)
    {
        Pmsg->revert = GROUPNOTEXIST;
        sqlite3_close(ppdb);
        return;
    }
    else
    {
        Pmsg->revert == GROUPEXIST;
        sqlite3_close(ppdb);
    }
}


/*获取群名称的回调函数 */
static int callback_get_group_name(void *para, int columnCount, char **columnValue, char **columnName)
{
    Msg *Pmsg = (Msg *)para;
    strcpy(Pmsg->group, columnValue[1]);
    return 0;
}

/*加入群聊 */
void join_group_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};
    int flag = 0;

    ret = sqlite3_open("group_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    check_group_exist(Pmsg);
    /*没有此群 */
    if (Pmsg->revert == GROUPNOTEXIST)
    {
        Pmsg->revert = GROUPNOTEXIST;
        mysend(Pmsg->fd, Pmsg);
        sqlite3_close(ppdb);
        return;
    }
    /*获取群名称 */
    sprintf(sql, "select * from group_list where id = '%d';", Pmsg->group_id);
    ret = sqlite3_exec(ppdb, sql, callback_get_group_name, Pmsg, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    /*向加入的群表中添加用户信息 */
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "insert into '%d' values ('%d', '%s', '0', '0');", Pmsg->group_id, Pmsg->id, Pmsg->name);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    /*向用户加入的群表中添加群组信息 */
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "create table if not exists '%d' (id unsigned int primary key, name text);", Pmsg->id);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    memset(sql, 0, sizeof(sql));
    sprintf(sql, "insert into '%d' values ('%d', '%s');", Pmsg->id, Pmsg->group_id, Pmsg->group);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    Pmsg->revert = JOINSUCCESS;
    mysend(Pmsg->fd, Pmsg);

    sqlite3_close(ppdb);
}

/*群组离线消息 */
void offline_group_msg_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);
    //debug_msg("Pmsg->name = %s\n", Pmsg->name);

    sprintf(sql, "insert into chat_history values ('%s', '%d', '%s', '%d', '%s', '0', '%d', '%s');", Pmsg->name, Pmsg->id, Pmsg->toname, Pmsg->toid, Pmsg->msg, Pmsg->group_id, Pmsg->group); //向自己好友表中插入对方id和用户名
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}


void online_group_msg_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};

    ret = sqlite3_open("user_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);
    //debug_msg("Pmsg->name = %s\n", Pmsg->name);

    sprintf(sql, "insert into chat_history values ('%s', '%d', '%s', '%d', '%s', '1', '%d', '%s');", Pmsg->name, Pmsg->id, Pmsg->toname, Pmsg->toid, Pmsg->msg, Pmsg->group_id, Pmsg->group); //向自己好友表中插入对方id和用户名
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}

static int chat_group_callback(void *para, int columnCount, char **columnValue, char **columnName)
{
    Msg *Pmsg = (Msg *)para;
    int ret;
    extern Link head;
    Pmsg->toid = atoi(columnValue[0]);
    strcpy(Pmsg->toname, columnValue[1]);

    Link p = find_online_toid(head, Pmsg);
    debug_msg("%s : %d\n", __FILE__, __LINE__);
    debug_msg("Pmsg->revert = %d\n", Pmsg->revert);

    if (Pmsg->revert == ONLINEIN)
    {
        Pmsg->revert = CHATOK;
        mysend(Pmsg->fd, Pmsg); //返回信息

        Pmsg->revert = CHATGROUP;
        online_group_msg_db(Pmsg);
        debug_msg("%s : %d\n", __FILE__, __LINE__);
        debug_msg("p->fd = %d;p->name = %s;p->id = %d", p->fd, p->name, p->id);
        mysend(p->fd, Pmsg); //给目标发送信息
    }
    else
    {
        debug_msg("%s : %d\n", __FILE__, __LINE__);
        Pmsg->revert = ONLINEOUT;
        find_user_db(Pmsg);
        if (Pmsg->revert == EXIST)
        {
            debug_msg("%s : %d\n", __FILE__, __LINE__);
            offline_group_msg_db(Pmsg);
            Pmsg->revert = CHATOK;
            mysend(Pmsg->fd, Pmsg);
        }
        else //用户不存在
        {
            debug_msg("%s : %d\n", __FILE__, __LINE__);
            mysend(Pmsg->fd, Pmsg); //返回用户不存在提示
        }
    }

    return 0;
}

/*发送群消息 */
void chat_group_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};
    int flag = 0;
    debug_msg("Pmsg->name = %s\n", Pmsg->name);

    ret = sqlite3_open("group_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    check_group_exist(Pmsg);
    /*没有此群 */
    if (Pmsg->revert == GROUPNOTEXIST)
    {
        //Pmsg->revert = GROUPNOTEXIST;
        mysend(Pmsg->fd, Pmsg);
        sqlite3_close(ppdb);
        return;
    }
    /*获取群名称 */
    sprintf(sql, "select * from group_list where id = '%d';", Pmsg->group_id);
    ret = sqlite3_exec(ppdb, sql, callback_get_group_name, Pmsg, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select * from '%d' where id != '%d';", Pmsg->group_id, Pmsg->id);
    ret = sqlite3_exec(ppdb, sql, chat_group_callback, Pmsg, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    sqlite3_close(ppdb);
}

/*
    功能：先检查此群的合法性，再将用户从指定群组表中删除，同时从用户表中将群组信息删除
    参数：Pmsg: id, name, group_id
 */
void quit_group_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};
    int flag = 0;
    debug_msg("Pmsg->name = %s\n", Pmsg->name);

    ret = sqlite3_open("group_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    check_group_exist(Pmsg);
    /*没有此群 */
    if (Pmsg->revert == GROUPNOTEXIST)
    {
        //Pmsg->revert = GROUPNOTEXIST;
        mysend(Pmsg->fd, Pmsg);
        sqlite3_close(ppdb);
        return;
    }

    /*从群组表中删除用户 */
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "delete from '%d' where id = '%d';", Pmsg->group_id, Pmsg->id);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    /*在用户表中删除群组信息 */
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "delete from '%d' where id = '%d';", Pmsg->id, Pmsg->group_id);
    ret = sqlite3_exec(ppdb, sql, NULL, NULL, NULL);
    is_sqlite_ok(ret, ppdb);
    debug_msg("%s : %d\n", __FILE__, __LINE__);

    sqlite3_close(ppdb);
}

/*
    功能：先检测群ID的合法性，再根据群ID查表，返回群成员信息
    参数：Pmsg：group_id, fd
    发送：id, name, flag(群职务)
 */
static int show_groupMember_callback(void *para, int columnCount, char **columnValue, char **columnName)
{
    Msg *Pmsg = (Msg *)para;

    Pmsg->id = atoi(columnValue[0]);
    strcpy(Pmsg->name, columnValue[1]);
    Pmsg->flag = atoi(columnValue[3]);
    Pmsg->revert = SHOWGROUPMEMBER;

    mysend(Pmsg->fd, Pmsg);
    usleep(10);

    return 0;
}

void show_groupMember_db(Msg *Pmsg)
{
    sqlite3 *ppdb;
    int ret;
    char sql[1024] = {0};
    int flag = 0;
    debug_msg("Pmsg->name = %s\n", Pmsg->name);

    ret = sqlite3_open("group_info.db", &ppdb);
    is_sqlite_ok(ret, ppdb);

    check_group_exist(Pmsg);
    /*没有此群 */
    if (Pmsg->revert == GROUPNOTEXIST)
    {
        //Pmsg->revert = GROUPNOTEXIST;
        mysend(Pmsg->fd, Pmsg);
        sqlite3_close(ppdb);
        return;
    }

    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select * from '%d';", Pmsg->group_id);
    ret = sqlite3_exec(ppdb, sql, show_groupMember_callback, Pmsg, NULL);
    is_sqlite_ok(ret, ppdb);

    sqlite3_close(ppdb);
}