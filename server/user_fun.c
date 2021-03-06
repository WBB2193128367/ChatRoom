#include "server.h"
#include "package.h"

Link head = NULL;
Link newnode;

void user_reg(Msg *Pmsg, int fd)
{
    int ret;

    is_name_exist(Pmsg);

    if (Pmsg->revert == EXIST)
    {
        mysend(fd, Pmsg);
    }
    else if (Pmsg->revert == NOEXIST)
    {
        int id = reg_db(Pmsg);

        Pmsg->id = id;
        debug_msg("%s : %d\n", __FILE__, __LINE__);

        ret = send(fd, Pmsg, sizeof(Msg), 0);
        is_send_recv_ok(ret, "send error");
        debug_msg("%s : %d\n", __FILE__, __LINE__);
    }
}

void login(Msg *Pmsg, int fd)
{
    int id;
    int ret;

    ret = find_node(head, Pmsg);

    if (ret == ONLINEIN)
    {
        Pmsg->revert = ONLINEIN;
        mysend(fd, Pmsg);

        return;
    }

    log_db(Pmsg);

    if (Pmsg->revert == LOGINOK)
    {
        create_node(&newnode);
        newnode->fd = fd;
        newnode->id = Pmsg->id;
        strncpy(newnode->name, Pmsg->name, NAMESIZE);

        insert_head(&head, newnode);

        display_list(head);
    }

    mysend(fd, Pmsg);
}


void login_name(Msg *Pmsg, int fd)
{
    int ret;

    ret = find_node(head, Pmsg);

    if (ret == ONLINEIN)
    {
        Pmsg->revert = ONLINEIN;
        mysend(fd, Pmsg);

        return;
    }

    log_name_db(Pmsg);

    if (Pmsg->revert == LOGINOK)
    {
        create_node(&newnode);
        newnode->fd = fd;
        newnode->id = Pmsg->id;
        strncpy(newnode->name, Pmsg->name, NAMESIZE);

        insert_head(&head, newnode);

        display_list(head);
    }

    mysend(fd, Pmsg);
}


void sendall_login(int fd, Msg *Pmsg)
{
    Link p, q;
    int ret;

    p = head;
    q = NULL;

    while (p != NULL)
    {
        if (p->id == Pmsg->id)
        {
            q = p;
            break;
        }
        p = p->next;
    }

    p = head;

    while (p != NULL)
    {
        if (p != q)
        {
            Pmsg->revert = SENDLOGIN;
            mysend(p->fd, Pmsg);

            usleep(100);

            ret = send(p->fd, q, sizeof(Node), 0);
            is_send_recv_ok(ret, "send error");
        }

        p = p->next;
    }
}

void logout(int fd, Msg *Pmsg)
{
    Link p, q;
    int ret;

    p = head;
    q = NULL;

    while (p != NULL)
    {
        if (p->id == Pmsg->id)
        {
            q = p;
            break;
        }
        p = p->next;
    }

    p = head;
    while (p != NULL)
    {
        if (p != q)
        {
            Pmsg->revert = SENDLOGOUT;
            mysend(p->fd, Pmsg);
        }

        p = p->next;
    }

    Pmsg->revert = LOGOUT;

    mysend(fd, Pmsg);

    delete_node(&head, Pmsg);

    printf("用户退出成功\n");
}

void exit_client(int fd)
{
    close(fd);

    printf("客户端正常退出\n");

    pthread_exit(NULL);
}

void chat_to(Msg *Pmsg, int fd)
{
    int ret;

    ret = find_online(head, Pmsg);
    Pmsg->group_id = 0;
    if (ret == ONLINEIN)
    {
        Pmsg->revert = CHATOK;
        debug_msg("%s: %d >> %d,%s,%d,%s,%s\n", __FILE__, __LINE__, Pmsg->id, Pmsg->name, Pmsg->toid, Pmsg->toname, Pmsg->msg);
        mysend(fd, Pmsg); //返回信息

        Pmsg->revert = CHATTO;
        mysend(Pmsg->fd, Pmsg); //给目标发送信息
        online_msg_db(Pmsg);
    }
    else
    {
        find_user_db(Pmsg);
        if (Pmsg->revert == EXIST)
        {
            offline_msg_db(Pmsg);
            Pmsg->revert = CHATOK;
            mysend(fd, Pmsg); //返回信息
        }
        else
        {
            mysend(fd, Pmsg); //返回信息
        }
        
    }
}

void showOnlineFriend(int fd)
{

#if 1
    Link p = head;
    int ret;
    Msg msg;

    msg.revert = SHOWONLINE;

    mysend(fd, &msg);
    sleep(0.5);
#if 1
    while (p != NULL)
    {
        ret = send(fd, p, sizeof(Node), 0);
        is_send_recv_ok(ret, "send error");

        p = p->next;
    }

    Node newnode;

    newnode.id = 0;
    ret = send(fd, &newnode, sizeof(Node), 0);
    is_send_recv_ok(ret, "send error");
#endif
#endif
}

void add_friend(int fd, Msg *Pmsg)
{
    int ret;

    find_friend(Pmsg);
    if (Pmsg->revert == EXIST)
    {
        ;
    }
    else if (Pmsg->revert == NOEXIST)
    {
        find_online(head, Pmsg);
        if (Pmsg->revert == ONLINEIN)
        {
            Pmsg->revert = ADDFRIEND;
            mysend(Pmsg->fd, Pmsg);

            Pmsg->revert = NOEXIST;
        }
    }

    mysend(fd, Pmsg);
}

void ret_friend(int fd, Msg *Pmsg)
{
    //同意
    {
        toid_to_toname(Pmsg);

        add_friend_db(Pmsg);

        Pmsg->revert = RETFRIENDOK;

        mysend(fd, Pmsg);

        find_online(head, Pmsg);

        Pmsg->revert = ADDFRIENDOK;

        mysend(Pmsg->fd, Pmsg);
    }
}

void passwd(int fd, Msg *Pmsg)
{
    updata_passwd(Pmsg);
    Pmsg->revert = PASSWD;
    mysend(fd, Pmsg);
}

void chat_all(int fd, Msg *Pmsg)
{
    Link p;
    p = head;

    while (p != NULL)
    {
        //printf("%d\n", __LINE__);
        if (p->fd != fd)
        {
            Pmsg->revert = CHATALL;
            mysend(p->fd, Pmsg);
        }
        else
        {
            Pmsg->revert = CHATOK;
            mysend(fd, Pmsg);
        }

        p = p->next;
    }
}

void chat_to_id(int fd, Msg *Pmsg)
{
    int ret;
    Link p;

    p = find_online_toid(head, Pmsg);
    debug_msg("%s : %d\n", __FILE__, __LINE__);
    if (Pmsg->revert == ONLINEIN)
    {
        Pmsg->revert = CHATOK;
        mysend(fd, Pmsg); //返回信息

        Pmsg->revert = CHATTO;
        mysend(p->fd, Pmsg); //给目标发送信息
        online_msg_db(Pmsg);
        debug_msg("%s : %d\n", __FILE__, __LINE__);
    }
    else
    {
        Pmsg->revert = ONLINEOUT;
        find_user_db(Pmsg);
        debug_msg("%s: %d >> %d,%s,%d,%s,%s\n", __FILE__, __LINE__, Pmsg->id, Pmsg->name, Pmsg->toid, Pmsg->toname, Pmsg->msg);
        if (Pmsg->revert == EXIST)
        {
            offline_msg_db(Pmsg);
            Pmsg->revert = CHATOK;
            mysend(fd, Pmsg);
        }
        else
        {
            mysend(fd, Pmsg); //返回信息
            debug_msg("%s : %d\n", __FILE__, __LINE__);
        }
    }
}

void get_offline_msg(int fd, Msg *Pmsg)
{
    printf("get offline msg\n");
    get_offline_db(fd, Pmsg);
}

void create_group(int fd, Msg *Pmsg)
{
    debug_msg("create group\n");
    create_group_db(Pmsg);
    if (Pmsg->revert == GROUPOK)
    {
        mysend(fd, Pmsg);
    }
    else
    {
        Pmsg->revert = GROUPFAIL;
        mysend(fd, Pmsg);
    }
}

void show_group(int fd, Msg *Pmsg)
{
    debug_msg("this is show group!\n");
    Pmsg->fd = fd;

    show_group_db(Pmsg);
}

void join_group(int fd, Msg *Pmsg)
{
    debug_msg("This is join group!\n");
    Pmsg->fd = fd;
    join_group_db(Pmsg);
}

/*发送群聊信息 */
void chat_group(int fd, Msg *Pmsg)
{
    debug_msg("This is group chat\n");
    Pmsg->fd = fd;
    chat_group_db(Pmsg);
}

/*
    功能: 退出群组
    参数:   fd: 通讯描述符;
            Pmsg: id, name, group_id;
    
 */
void quit_group(int fd, Msg *Pmsg)
{
    debug_msg("This is group quit\n");
    Pmsg->fd = fd;
    quit_group_db(Pmsg);
    Pmsg->revert = QUITOK;
    mysend(fd, Pmsg);
}

/*
    功能：显示全体群成员
    参数：  fd: 通讯描述符;
            Pmsg: id, name, group_id;
 */
void show_groupMember(int  fd, Msg *Pmsg)
{
    debug_msg("This is show group member\n");
    Pmsg->fd = fd;
    show_groupMember_db(Pmsg);
}


void send_file(int fd, Msg *Pmsg)
{
    pid_t pid;
    int port;
    int ret;
    char port_s[6] = {0};

    port = get_port();

    Pmsg->flag = port;
    Pmsg->revert = RETPORT;
    mysend(fd, Pmsg);
    sprintf(port_s, "%d", port);

    pid = fork();

    if(pid > 0)
    {
        printf("pid = %d\n", pid);
    }
    else if(pid == 0)
    {
        int ret = execl("./server/server", "./server", port_s, NULL);
        if(ret == -1)
        {
            perror("execl error");
        }
    }
    
}