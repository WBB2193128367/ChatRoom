#include "client.h"
#include "package.h"

sqlite3 *db;
char name_global[NAMESIZE];
int id_global;


void group_handler(int fd)
{
    int choice;
    int ret;

    menu_group();

    while(1)
    {
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:     //显示自己加入的群组
                {
                    process_showGroup(fd);
                    break;
                }
            case 2:     //用群ID加入群组
                {
                    process_joinGroup(fd);
                    break;
                }
            case 3:     //退出群聊
                {
                    process_quitGroup(fd);
                    break;
                }
            case 4:     //创建群组
                {
                    process_creatGroup(fd);
                    break;
                }
            case 5:     //在群组中聊天
                {
                    process_chatGroup(fd);
                    break;
                }
            case 6:     //显示群用户
                {
                    process_showGroupMember(fd);
                    break;
                }
            case 9:     //返回上一层
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

    strcpy(name_global, name);      //保存用户名的全局变量
    
    open_db(&db);

    create_user_db(db, name);       //创建用户数据库

    pthread_t tid;

    pthread_create(&tid,NULL,read_test,(void *)(&fd));      //创建读消息线程

    sleep(1);

    menu_func(name);

    process_showOnline(fd);     //获取在线链表
    sleep(1);

    process_get_offline(fd);    //获取离线消息

    while(1)
    {
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:     //显示在线用户列表
                {
                    //process_showOnline(fd);
                    display_list(head);
                    break;
                }
            case 2:     //用户名私聊
                {
                    process_chat(fd, name);
                    break;
                }
            case 3:     //添加好友
                {
                    process_addFriend(fd, name);
                    break;
                }
            case 4:     //修改密码
                {
                    process_passwd(fd);
                    break;
                }
            case 5:     //退出登录
                {
                    logout(fd, name);
                    return;
                }
            case 6:     //回应好友请求
                {
                    process_retFriend(fd, name);
                    break;
                }
            case 7:     //给全体发送信息
                {
                    process_chatall(fd, name);
                    break;
                }
            case 8:     //ID私聊
                {
                    process_chattoid(fd, name);
                    break;
                }
            case 9:     //群相关功能
                {
                    group_handler(fd);
                    menu_func(name);
                    break;
                }
            case 10:
                {
                    menu_func(name);
                    break;
                }
            case 11:
                {
                    process_sendFile(fd);
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
            case 1:     //ID登录
            {
                login(name, fd);
                if(strlen(name) != 0)
                {
                    user_handler(name, fd);
                }
                break;
            }
            case 2:     //注册
            {
                user_reg(fd);
                break;
            }
            case 3:     //用户名登录
            {
                login_name(name, fd);
                if(strlen(name) != 0)
                {
                    user_handler(name, fd);
                }
                break;
            }
            case 4:     //客户端退出
            {
                exit_client(fd);
                break;
            }
            default :
            {
                printf("请输入正确的操作!\n");
                char c_tmp;
                while((c_tmp = getchar() != '\n') && c_tmp != EOF);
                break;
            }
        }
    }
}