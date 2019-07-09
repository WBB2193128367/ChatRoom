#include "server.h"


int main()
{
    int sockfd;

    InitDataBase();

    sockfd = InitNet();

    handler(sockfd);
    
    return 0;
}