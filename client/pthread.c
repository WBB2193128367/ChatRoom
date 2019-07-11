#include "client.h"
#include "package.h"


void * read_test(void * arg)
{
    int fd = *((int *)arg);
    int ret;
    Msg msg;

	pthread_detach(pthread_self());

    while(1)
    {
        myrecv(fd, &msg);

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
            case SHOWONLINE:
                {
                    Link p, head;
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
            default:
                printf("错误\n");
                break;
        }
    }
}