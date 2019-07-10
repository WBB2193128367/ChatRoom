#include "server.h"

extern Link head;

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
		is_send_recv_ok(ret, "recv error");

		if(ret == 0)
		{
			logout(fd);
			exit_client(fd);
		}

		switch(msg.cmd)
		{
			case REG:
			{
				user_reg(&msg, fd);
				break;	
			}
			case LOGIN:
			{
				ret = login(&msg, fd);
				break;
			}
			case LOGOUT:
			{
				logout(fd);
				break;
			}
			case EXIT:
			{
				exit_client(fd);
				break;
			}
			case SHOWONLINE:
			{
				showOnlineFriend(fd);
				break;
			}
		}
	}
}