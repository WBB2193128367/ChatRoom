#include "client.h"


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
    "***       1 显示在线好友         2 私聊                    ***\n"
    "***                                                      ***\n"
    "***       3 群聊                4 退出                    ***\n"
    "                                                            \n"
    "  ****      Hello, sir!  What can I do for you ?      ****  \n", name);

}


void user_handler(char * name, int fd)
{
    int choice;
    int ret;

    menu_func(name);

    while(1)
    {
        

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
            {
                showOnlineFriend(fd);
                break;
            }
            case 4:
            {
                logout(fd);
                return;
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

    while(1)
    {
        menu_user();

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
            {
                char name[NAMESIZE] = {0};
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
            }
            default :
            {
                printf("请输入正确的操作!\n");
                break;
            }
        }
    }
}