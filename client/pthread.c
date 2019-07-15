#include "client.h"
#include "package.h"

Link p, head;


void * read_test(void * arg)
{
    debug_msg("recv start!\n");
    extern sqlite3 *db;
    extern char name_global[NAMESIZE];
    int fd = *((int *)arg);
    int ret;
    Msg msg;

	pthread_detach(pthread_self());

    while(1)
    {
        myrecv(fd, &msg);

        debug_msg("revert = %d\n", msg.revert);

        switch (msg.revert)
        {
            case CHATOK:
                {
                    printf("发送成功\n");
                    break;
                }
            case ONLINEOUT:
                {
                    printf("对方不在线\n");
                    break;
                }
            case CHATTO:
                {
                    printf("%s 给你发了: %s\n", msg.name, msg.msg);
                    break;
                }
            case CHATALL:
                {
                    printf("%s 群发了消息：%s\n", msg.name, msg.msg);
                    break;
                }
            case SHOWONLINE:
                {
                    p = head = NULL;
                    Link newnode;
#if 1
                    while(1)
                    {
                        create_node(&newnode);

                        ret = recv(fd, newnode, sizeof(Node), 0);
                        if(ret < 0)
                        {
                            perror("recv error");
                            exit(1);
                        }

                        if(newnode->id == 0)
                        {
                            break;
                        }
                        else if(newnode->id != 0)
                        {
                            if(head == NULL)
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
#endif
                    display_list(head);
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
            default:
                printf("错误\n");
                break;
        }
    }
}