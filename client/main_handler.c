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


void user_handler(int fd)
{
    while(1)
    {
        
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
                ret = login(fd);
                if(ret == LOGINOK)
                {
                    user_handler(fd);
                }
                else if(ret == LOGINFAIL)
                {
                    continue;
                }
                break;
            }
            case 2:
            {
                user_reg(fd);
                break;
            }
            
        }
    }
}