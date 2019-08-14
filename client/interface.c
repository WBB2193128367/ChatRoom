#include "client.h"
#include "package.h"


void insert_id()
{
	system("reset");
    printf("\n");
	printf("\n");
	printf("\033[;31m \t\t\t\t\t________________________________________\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|       智能聊天室欢迎您的登录         |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|       请输入用ID：                   |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|       请输入密码：                   |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|______________________________________|\n\033[0m");
	printf("\033[5A");
	printf("\033[60C");
}

void insert_log()
{
	system("reset");
    printf("\n");
	printf("\n");
	printf("\033[;31m \t\t\t\t\t________________________________________\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|       智能聊天室欢迎您的登录         |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|     请输入用户名：                   |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|       请输入密码：                   |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|______________________________________|\n\033[0m");
	printf("\033[5A");
	printf("\033[60C");
}


void insert_reg()
{
	system("reset");
    printf("\n");
	printf("\n");
	printf("\033[;31m \t\t\t\t\t________________________________________\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|       智能聊天室感谢您的注册         |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|     请输入用户名：                   |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|       请输入密码：                   |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|   请再次输入密码：                   |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
    printf("\033[;31m \t\t\t\t\t|                                      |\n\033[0m");
	printf("\033[;31m \t\t\t\t\t|______________________________________|\n\033[0m");
	printf("\033[7A");
	printf("\033[60C");
}


void menu_group()
{
    system("reset");
    extern char name_global[NAMESIZE];

    printf(
    "\n"
    "\n"
    "\033[;31m \t\t************************************************************\n\033[0m"
    "\033[;31m \t\t**********                                        **********\n\033[0m"
    "\033[;31m \t\t**********                                        **********\n\033[0m"
    "\033[;31m \t\t**********                WELCOME                 **********\n\033[0m"
    "\033[;31m \t\t**********                                        **********\n\033[0m"
    "\033[;31m \t\t**********                %-20s    **********\n\033[0m"
    "\033[;31m \t\t**********                                        **********\n\033[0m"
    "\033[;31m \t\t************************************************************\n\033[0m"
    "\033[;31m \t\t                                                            \n\033[0m"
    "\033[;31m \t\t***       1 显示加入的群聊      2 加入群聊               ***\n\033[0m"
    "\033[;31m \t\t***                                                      ***\n\033[0m"
    "\033[;31m \t\t***       3 退出群聊            4 创建群聊               ***\n\033[0m"
    "\033[;31m \t\t***                                                      ***\n\033[0m"
    "\033[;31m \t\t***       5 群聊                6 显示群成员             ***\n\033[0m"
    "\033[;31m \t\t***                                                      ***\n\033[0m"
    "\033[;31m \t\t***       9 退出群聊功能                                 ***\n\033[0m"
    "\033[;31m \t\t                                                            \n\033[0m"
    "\033[;31m \t\t  ****      Hello, sir!  What can I do for you ?      ****  \n\033[0m", name_global);
}



void menu_user()
{
    system("reset");

    printf(
    "\n"
    "\n"
    "\033[;34m \t\t\t************************************************************\n\033[0m"
    "\033[;34m \t\t\t**********                                        **********\n\033[0m"
    "\033[;34m \t\t\t**********                                        **********\n\033[0m"
    "\033[;34m \t\t\t**********                WELCOME                 **********\n\033[0m"
    "\033[;34m \t\t\t**********                                        **********\n\033[0m"
    "\033[;34m \t\t\t**********                                        **********\n\033[0m"
    "\033[;34m \t\t\t************************************************************\n\033[0m"
    "                                                            \n"
    "\033[;34m \t\t\t***       1 ID登陆              2 注册                   ***\n\033[0m"
    "\033[;34m \t\t\t***                                                      ***\n\033[0m"
    "\033[;34m \t\t\t***       3 用户名登录          4 退出                   ***\n\033[0m"
    "\033[;34m \t\t\t                                                            \n\033[0m"
    "\033[;34m \t\t\t  ****      Hello, sir!  What can I do for you ?      ****  \n\033[0m");

}



void menu_func(char * name)
{
    system("reset");

    printf(
    "\033[;32m \t\t************************************************************\n\033[0m"
    "\033[;32m \t\t**********                                        **********\n\033[0m"
    "\033[;32m \t\t**********                WELCOME                 **********\n\033[0m"
    "\033[;32m \t\t**********                                        **********\n\033[0m"
    "\033[;32m \t\t**********                                        **********\n\033[0m"
    "\033[;32m \t\t**********                %-20s    **********\n\033[0m"
    "\033[;32m \t\t************************************************************\n\033[0m"
    "\033[;32m \t\t                                                            \n\033[0m"
    "\033[;32m \t\t***       1 显示在线好友        2 私聊                   ***\n\033[0m"
    "\033[;32m \t\t***                                                      ***\n\033[0m"
    "\033[;32m \t\t***       3 添加好友            4 修改密码               ***\n\033[0m"
    "\033[;32m \t\t***                                                      ***\n\033[0m"
    "\033[;32m \t\t***       5 退出                6 回应添加对方为好友     ***\n\033[0m"
    "\033[;32m \t\t***                                                      ***\n\033[0m"
    "\033[;32m \t\t***       7 群发                8 id私聊                 ***\n\033[0m"
    "\033[;32m \t\t***                                                      ***\n\033[0m"
    "\033[;32m \t\t***       9 群功能              10 显示主界面            ***\n\033[0m"
    "\033[;32m \t\t***                                                      ***\n\033[0m"
    "\033[;32m \t\t***       11 发文件                                      ***\n\033[0m"
    "                                                            \n"
    "\033[;32m \t\t  ****      Hello, sir!  What can I do for you ?      ****  \n\033[0m", name);

}