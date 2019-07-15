#include "client.h"
#include "package.h"

sqlite3 *db;
char name_global[NAMESIZE];
int id_global;

void menu_user()
{
    system("clear");

    printf(
    "************************************************************\n"
    "**********                                        **********\n"
    "**********                                        **********\n"
    "**********                WELCOME                 **********\n"
    "**********                                        **********\n"
    "**********                                        **********\n"
    "************************************************************\n"
    "                                                            \n"
    "***       1 登陆                2 注册                    ***\n"
    "***                                                      ***\n"
    "***       3 退出                                         ***\n"
    "                                                            \n"
    "  ****      Hello, sir!  What can I do for you ?      ****  \n");

}


void menu_func(char * name)
{
    system("clear");

    printf(
    "************************************************************\n"
    "**********                                        **********\n"
    "**********                WELCOME                 **********\n"
    "**********                                        **********\n"
    "**********                                        **********\n"
    "**********                %-20s    **********\n"
    "************************************************************\n"
    "                                                            \n"
    "***       1 显示在线好友          2 私聊                    ***\n"
    "***                                                      ***\n"
    "***       3 添加好友             4 修改密码                 ***\n"
    "***                                                      ***\n"
    "***       5 退出                 6 回应添加对方为好友        ***\n"
    "***                                                      ***\n"
    "***       7 群发                                          ***\n"
    "                                                            \n"
    "  ****      Hello, sir!  What can I do for you ?      ****  \n", name);

}


void user_handler(char * name, int fd)
{
    int choice;
    int ret;
    extern Link head;
    
    open_db(&db);

    create_user_db(db, name);

    pthread_t tid;

    pthread_create(&tid,NULL,read_test,(void *)(&fd));

    sleep(2);

    menu_func(name);

    process_showOnline(fd);

    while(1)
    {
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                {
                    //process_showOnline(fd);
                    display_list(head);
                    break;
                }
            case 2:
                {
                    process_chat(fd, name);
                    break;
                }
            case 3:
                {
                    process_addFriend(fd, name);
                    break;
                }
            case 4:
                {
                    process_passwd(fd);
                    break;
                }
            case 5:
                {
                    logout(fd, name);
                    return;
                }
            case 6:
                {
                    process_retFriend(fd, name);
                    break;
                }
            case 7:
                {
                    process_chatall(fd, name);
                    break;
                }
            default :
            {
                printf("请输入正确的操作!\n");
                break;
            }
        } 
    }
}


void main_handler(int fd)
{
    int choice;
    int ret;
    char name[NAMESIZE] = {0};

    while(1)
    {
        menu_user();

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
            {
                
                login(name, fd);
                if(strlen(name) != 0)
                {
                    user_handler(name, fd);
                }
                break;
            }
            case 2:
            {
                user_reg(fd);
                break;
            }
            case 3:
            {
                exit_client(fd);
                break;
            }
            default :
            {
                printf("请输入正确的操作!\n");
                break;
            }
        }
    }
}