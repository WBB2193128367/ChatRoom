#include "client.h"
#include "package.h"

sqlite3 *db;
char name_global[NAMESIZE];
int id_global;


void group_handler(int fd)
{
    int choice;
    int ret;

    while(1)
    {
        menu_group();

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                {
                    process_showGroup(fd);
                    break;
                }
            case 2:
                {
                    process_joinGroup(fd);
                    break;
                }
            case 3:
                {
                    break;
                }
            case 4:
                {
                    process_creatGroup(fd);
                    break;
                }
            case 5:
                {
                    process_chatGroup(fd);
                    break;
                }
            case 9:
                {
                    return;
                    break;
                }
            default:
                {
                    printf("错误\n");
                    break;
                }
        }

        sleep(2);
    }
}


void user_handler(char * name, int fd)
{
    int choice;
    int ret;
    extern Link head;

    strcpy(name_global, name);
    
    open_db(&db);

    create_user_db(db, name);

    pthread_t tid;

    pthread_create(&tid,NULL,read_test,(void *)(&fd));

    sleep(1);

    menu_func(name);

    process_showOnline(fd);
    sleep(1);

    process_get_offline(fd);

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
            case 8:
                {
                    process_chattoid(fd, name);
                    break;
                }
            case 9:
                {
                    group_handler(fd);
                    menu_func(name);
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
        sleep(1);
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
                
                break;
            }
            case 4:
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