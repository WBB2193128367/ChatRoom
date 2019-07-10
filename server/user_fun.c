#include "server.h"

Link head = NULL;
Link newnode;


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
    is_send_recv_ok(ret, "recv error");

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
    is_send_recv_ok(ret, "send error");
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


int login(Msg * Pmsg, int fd)
{
    Msg msg = *(Pmsg);
    int id;
    char sql[1024] = {0};
    create_node(&newnode);

    sqlite3 *ppdb;
    char name[NAMESIZE];
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
    is_send_recv_ok(ret, "recv error");

    strncpy(psw, msg.msg, PSWSIZE);

    sprintf(sql, "select * from user where id = '%d' and passwd = '%s';", id, psw);
    ret = sqlite3_exec(ppdb, sql, login_callback, &i_condition_login, NULL);
    if(ret != SQLITE_OK)
    {
        printf("select fail : %s\n", sqlite3_errmsg(ppdb));
    }

    ret = sqlite3_exec(ppdb, sql, login_callback_name, &name, NULL);
    if(ret != SQLITE_OK)
    {
        printf("select fail : %s\n", sqlite3_errmsg(ppdb));
    }

    if(i_condition_login == 1)
    {
        msg.cmd = LOGINOK;
        strncpy(msg.msg, name, NAMESIZE);

        ret = send(fd, &msg, sizeof(msg), 0);
        is_send_recv_ok(ret, "send error");

        newnode->fd = fd;
        newnode->id = id;
        strncpy(newnode->name, name, NAMESIZE);

        insert_head(&head, newnode);

        display_list(head);

        return LOGINOK;
    }
    else
    {
        msg.cmd = LOGINFAIL;
        ret = send(fd, &msg, sizeof(msg), 0);
        is_send_recv_ok(ret, "send error");

        return LOGINFAIL;
    }
    
}



void logout(int fd)
{
    delete_node(&head, fd);

    printf("用户退出成功\n");
}



void exit_client(int fd)
{
    close(fd);

    printf("客户端正常退出\n");

    pthread_exit(NULL);
}



void showOnlineFriend(int fd)
{
    #if 0
    Link p = head;
    int ret;

    while(p != NULL)
    {
        ret = send(fd, p, sizeof(Node), 0);
        is_send_recv_ok(ret, "send error");

        p = p->next;
    }

    Node newnode;

    newnode.id = 0;
    ret = send(fd, &newnode, sizeof(Node), 0);
    is_send_recv_ok(ret, "send error");

    display_list(head);
    #endif
}