#include "client.h"
#include "package.h"

Link p, head;
int port = 0;

void *read_test(void *arg)
{
    debug_msg("recv start!\n");
    extern sqlite3 *db;
    extern char name_global[NAMESIZE];
    int fd = *((int *)arg);
    int ret;
    Msg msg;

    pthread_detach(pthread_self());

    while (1)
    {
        memset(&msg, 0, sizeof(msg));
        myrecv(fd, &msg);

        debug_msg("revert = %d\n", msg.revert);

        switch (msg.revert)
        {
            case CHATOK:
            {
                printf("发送成功\n");
                if (msg.group_id == 0)
                {
		debug_msg("%s : %d\n", __FILE__, __LINE__);
                    history_msg_person_db(db, &msg);
                }
                else
                {
		debug_msg("%s : %d\n", __FILE__, __LINE__);
                    history_msg_group_db(db, &msg);
                }

                break;
            }
            case ONLINEOUT:
            {
                printf("对方不在线\n");
                break;
            }
            case CHATTO:
            {
                printf("%s 给你发了:\n\t%s\n", msg.name, msg.msg);
                history_msg_person_db(db, &msg);
                break;
            }
            case CHATALL:
            {
                printf("%s 群发了消息：%s\n", msg.name, msg.msg);
                break;
            }
            case SHOWONLINE:
            {
                read_showOnline(fd);
                break;
            }
            case LOGOUT:
            {
                pthread_exit(NULL);
                break;
            }
            case ADDFRIEND:
            {
                printf("%s(%d) 请求添加您为好友\n", msg.name, msg.id);

                //insert_friend_db(db, &msg);

                break;
            }
            case ADDFRIENDOK:
            {
                printf("%s(%d) 同意了您的好友请求\n", msg.name, msg.id);

                //添加到数据库
                insert_addfriend_db(db, &msg);

                break;
            }
            case RETFRIENDOK:
            {
                printf("您同意了对方的好友请求\n");

                //添加到数据库
                insert_retfriend_db(db, &msg);

                break;
            }
            case READD:
            {
                printf("添加成功\n");
                //insert_friend_db(db, &msg);
                break;
            }
            case PASSWD:
            {
                printf("修改成功\n");
                break;
            }
            case EXIST:
            {
                printf("对方已经是您的好友了！\n");
                break;
            }
            case NOEXIST:
            {
                printf("发送成功\n");
                break;
            }
            case SENDLOGIN:
            {
                Link newnode;
                create_node(&newnode);

                ret = recv(fd, newnode, sizeof(Node), 0);
                is_send_recv_ok(ret, "recv error");

                insert_head(&head, newnode);

                printf("%s 上线了!\n", newnode->name);
                break;
            }
            case SENDLOGOUT:
            {
                printf("%s 下线了\n", msg.name);
                delete_node(&head, &msg);
                break;
            }
            case OFFLINE:
            {
                //printf("%s(%d):\n\t%s\n", msg.name, msg.id, msg.msg);
                printf_msg(&msg);
                break;
            }
            case SHOWGROUP:
            {
                read_showGroup(fd, &msg);
                break;
            }
            case GROUPFAIL:
            {
                printf("群创建失败!\n");
                break;
            }
            case GROUPOK:
            {
                printf("您的群号为:%d, 群名称为:%s\n", msg.group_id, msg.group);
                break;
            }
            case JOINSUCCESS:
            {
                printf("加入群聊成功！\n");
                break;
            }
            case GROUPNOTEXIST:
            {
                printf("群组不存在！\n");
                break;
            }
            case CHATGROUP:
            {
                printf_msg(&msg);
                break;
            }
            case QUITOK:
            {
                printf("您以从 %d 中退出\n", msg.group_id);
                break;
            }
            case SHOWGROUPMEMBER:
            {
                printf_groupMember(&msg);
                break;
            }
            case RETPORT:
            {  
                port = msg.flag;
                printf("port = %d\n", port);
                break;
            }
            default:
            {
                printf("错误\n");
                //printf("%s, %d, %s, %d, %s, %d\n", msg.name, msg.id, msg.toname, msg.toid, msg.msg, msg.flag);
                break;
            }
        }
    }
}

void read_showOnline(int fd)
{
    p = head = NULL;
    Link newnode;
    int ret;
    while (1)
    {
        create_node(&newnode);

        ret = recv(fd, newnode, sizeof(Node), 0);
        if (ret < 0)
        {
            perror("recv error");
            exit(1);
        }

        if (newnode->id == 0)
        {
            break;
        }
        else if (newnode->id != 0)
        {
            if (head == NULL)
            {
                head = newnode;
                head->next = NULL;
                p = head;
            }
            else
            {
                (p->next) = newnode;
                newnode->next = NULL;
            }
        }
    }
    display_list(head);
}

void read_showGroup(int fd, Msg *Pmsg)
{
    printf("\t>>>%s (%d)\n", Pmsg->group, Pmsg->group_id);
}

void printf_msg(Msg *Pmsg)
{
    extern sqlite3 *db;
    if (Pmsg->group_id != 0)
    {
        printf("\t>> %s(%d) %s(%d) : %s\n", Pmsg->group, Pmsg->group_id, Pmsg->name, Pmsg->id, Pmsg->msg);
        history_msg_group_db(db, Pmsg);
    }
    else
    {
        printf("%s(%d):\n\t%s\n", Pmsg->name, Pmsg->id, Pmsg->msg);
        history_msg_person_db(db, Pmsg);
    }
}


void printf_groupMember(Msg *Pmsg)
{
    if(Pmsg->flag == 0) //普通群员
    {
        printf("\033[1;33m\t\t>>>%s (%d)\n\033[0m", Pmsg->name, Pmsg->id);
    }
    else if(Pmsg->flag == 1) //管理员
    {
        printf("\033[0;35m\t\t>>>%s (%d) (管理员)\n\033[0m", Pmsg->name, Pmsg->id);
    }
    else if(Pmsg->flag == 2)    //群主
    {
        printf("\033[0;31m\t\t>>%s (%d) (群主)\n\033[0m", Pmsg->name, Pmsg->id);
    }
}