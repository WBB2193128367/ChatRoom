#include "server.h"
#include "package.h"

int fd[CLIENT_MAX_SIZE];
pthread_t tid;

void handler(int sockfd)
{
    struct sockaddr_in client_addr;

    int i = 0;

    int addr_length = sizeof(client_addr);

    while(1)
    {
        for(i = 0; i < CLIENT_MAX_SIZE; i++)
        {
            if(0 == fd[i])
            break;
        }

        fd[i] = accept(sockfd, (struct sockaddr *)&client_addr, &addr_length);

	    printf("接受客户端连接 %d\n", fd[i]);

        int ret = pthread_create(&tid, NULL, server_child, &fd[i]);// 启动线程处理连接（一个线程对应一个客户端）
		if (-1 == ret)
		{
		    perror("pthread_create");
		    exit(1);
		}
    }
}