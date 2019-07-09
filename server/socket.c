#include "server.h"


int InitNet(void)
{
    struct sockaddr_in server_addr;//服务器信息
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (-1 == sockfd)
    {
        perror("sockfd");
	    exit(1);
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0 , sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);//nanopi的IP地址
	
	int ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	
	if(-1 == ret)
    {
        perror("bind");
	    exit(1);
    }
    
	ret = listen(sockfd, 10);
	if(-1 == ret)
	{
		perror("listen");
		exit(1);
	}
	return sockfd;
}