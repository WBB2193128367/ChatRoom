#include "client.h"


int InitNet(void)
{
    int ret;
    int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("socket error");
		exit(-1);
	}

	struct sockaddr_in addr;
                         
	addr.sin_family = AF_INET;    
	addr.sin_port    = htons(PORT);//服务器程序的端口
	addr.sin_addr.s_addr = inet_addr(IP);//服务器的ip地址

	ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	if(ret < 0)
	{
		perror("connect error");
		exit(-1);
	}

    return sockfd;
}