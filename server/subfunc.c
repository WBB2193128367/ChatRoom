#include "server.h"

void is_send_recv_ok(int ret, char *str)
{
	if(ret < 0)
	{
		perror(str);
	}
}