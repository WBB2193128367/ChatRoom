#include "data.h"

pthread_t tid;
int fd[CLIENT_MAX_SIZE];

int main(int argc, char **argv)
{   
    if(argc < 1)
    {
        printf("use ./server + 端口号\n");
        exit(0);
    }

    int port = atoi(argv[1]);

    /*初始化server，监听请求*/
    int listenfd = Server_init(port);
    socklen_t sockaddr_len = sizeof(struct sockaddr);

    /*epoll*/
    static struct epoll_event ev, events[EPOLL_SIZE];
	int epfd = epoll_create(EPOLL_SIZE);
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    while(1){
        int events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        int i=0;

        /*接受连接，添加work到work-Queue*/
        for(; i<events_count; i++){
            if(events[i].data.fd == listenfd)
            {
                int connfd;
                struct sockaddr_in  clientaddr;
                while( ( connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &sockaddr_len) ) > 0 )
				{
				    printf("EPOLL: Received New Connection Request---connfd= %d\n",connfd);
					struct args *p_args = (struct args *)malloc(sizeof(struct args));
                    p_args->fd = connfd;
                    p_args->recv_finfo = recv_fileinfo;
                    p_args->recv_fdata = recv_filedata;

                    /*添加work到work-Queue*/
                    //tpool_add_work(worker, (void*)p_args);
                    int ret = pthread_create(&tid, NULL, worker, (void*)p_args);// 启动线程处理连接（一个线程对应一个客户端）
                    if (-1 == ret)
                    {
                        perror("pthread_create");
                        exit(1);
                    }
				}
            }
        }
    }

    return 0;
}