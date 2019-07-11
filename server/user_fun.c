#include "server.h"
#include "package.h"

Link head = NULL;
Link newnode;



void user_reg(Msg *Pmsg, int fd)
{
    Msg msg = *(Pmsg);
    int ret;

    int id = reg_db(&msg);

    memset(&msg, 0, sizeof(msg));

    ret = send(fd, &msg, sizeof(msg), 0);
    is_send_recv_ok(ret, "send error");
}






void login(Msg * Pmsg, int fd)
{
    Msg msg = *(Pmsg);
    int id;
    int ret;

    ret = find_node(head, msg.id);

    if(ret == ONLINEIN)
    {
        msg.revert = ONLINEIN;
        mysend(fd, &msg);

        return;
    }
    
    create_node(&newnode);

    log_db(&msg);

    if(msg.revert == LOGINOK)
    {
        newnode->fd = fd;
        newnode->id = msg.id;
        strncpy(newnode->name, msg.name, NAMESIZE);

        insert_head(&head, newnode);

        display_list(head);
    }

    mysend(fd, &msg);
}



void logout(int fd)
{
    Msg msg;

    msg.revert = LOGOUT;

    mysend(fd, &msg);

    delete_node(&head, fd);

    printf("用户退出成功\n");
}



void exit_client(int fd)
{
    close(fd);

    printf("客户端正常退出\n");

    pthread_exit(NULL);
}




void chat_to(Msg * Pmsg, int fd)
{
    int ret;

    ret = find_online(head, Pmsg);
    if(ret == ONLINEIN)
    {
        Pmsg->revert = CHATOK;
        mysend(fd, Pmsg);//返回信息

        Pmsg->revert = CHATTO;
        mysend(Pmsg->fd, Pmsg);//给目标发送信息
    }
    else
    {
        Pmsg->revert = ONLINEOUT;
        mysend(fd, Pmsg);//返回信息
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
        debug_msg("%s : %d\n", __FILE__, __LINE__);
#if 1
    while(p != NULL)
    {
        ret = send(fd, p, sizeof(Node), 0);
        is_send_recv_ok(ret, "send error");
        debug_msg("%s : %d\n", __FILE__, __LINE__);

        p = p->next;
    }
        debug_msg("%s : %d\n", __FILE__, __LINE__);

    Node newnode;

    newnode.id = 0;
    ret = send(fd, &newnode, sizeof(Node), 0);
    is_send_recv_ok(ret, "send error");
#endif
    #endif
}