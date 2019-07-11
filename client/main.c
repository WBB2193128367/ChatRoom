#include "client.h"
#include "package.h"


int main(void)
{
    int sockfd;

    sockfd = InitNet();

    printf("服务器连接成功\n");

    main_handler(sockfd);
    
    return 0;
}