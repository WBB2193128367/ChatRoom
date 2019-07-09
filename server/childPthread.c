#include "server.h"


void user_handler(int fd)
{

}


void *server_child(void * arg)
{

	int fd = *((int *)arg);
	Msg msg;
	int ret;

	pthread_detach(pthread_self());

	printf("接受客户端连接 %d\n", fd);

	while(1)
	{
		memset(&msg, 0, sizeof(msg));

		ret = recv(fd, &msg, sizeof(msg), 0);
		if(ret < 0)
		{
			perror("recv error");
		}

		if(ret == 0)
		{
			printf("客户端%d断开连接\n", fd);
			pthread_exit(NULL);
		}

    	debug_msg("%s : %d\n", __FILE__, __LINE__);

		switch(msg.cmd)
		{
			case REG:
			{

    			debug_msg("%s : %d\n", __FILE__, __LINE__);
				user_reg(&msg, fd);
				break;	
			}
			case LOGIN:
			{
				ret = login(&msg, fd);

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
		}
	}
}