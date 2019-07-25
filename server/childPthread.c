#include "server.h"
#include "package.h"


extern Link head;

void *server_child(void * arg)
{

	int fd = *((int *)arg);
	Msg msg;
	int ret;
	int id;
	char name[NAMESIZE];

	pthread_detach(pthread_self());

	printf("接受客户端连接 %d\n", fd);

	while(1)
	{
		memset(&msg, 0, sizeof(msg));
		msg.fd = fd;

		ret = recv(fd, &msg, sizeof(Msg), 0);
		is_send_recv_ok(ret, "recv error");

		if(ret == 0)
		{
			msg.id = id;
			strncpy(msg.name, name, NAMESIZE);
			logout(fd, &msg);
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
				login(&msg, fd);
				id = msg.id;
				strncpy(name, msg.name, NAMESIZE);
				debug_msg("%s\n", name);
				//sleep(2);
				sendall_login(fd, &msg);
				break;
			}
			case LOGOUT:
			{
				logout(fd, &msg);
				break;
			}
			case EXIT:
			{
				exit_client(fd);
				break;
			}
			case CHATTO:
			{
				chat_to(&msg, fd);
				break;
			}
			case SHOWONLINE:
			{
				debug_msg("show online\n");
				showOnlineFriend(fd);
				break;
			}
			case ADDFRIEND:
			{
				add_friend(fd, &msg);
				break;
			}
			case RETFRIEND:
			{
				ret_friend(fd, &msg);
				break;
			}
			case PASSWD:
			{
				passwd(fd, &msg);
				break;
			}
			case CHATALL:
			{
				chat_all(fd, &msg);
				break;
			}
			case CAHTTOID:
			{
				chat_to_id(fd, &msg);
				break;
			}
			case GETOFFLINE:
			{
				get_offline_msg(fd, &msg);
				break;
			}
			case SHOWGROUP:
			{
				show_group(fd, &msg);
				break;
			}
			case JOINGROUP:
			{
				join_group(fd, &msg);
				break;
			}
			case CREATGROUP:
			{
				create_group(fd, &msg);
				break;
			}
			case CHATGROUP:
			{
				chat_group(fd, &msg);
				break;
			}
		}
	}
}