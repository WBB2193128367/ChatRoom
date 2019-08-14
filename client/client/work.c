#include "data.h"


/*结构体长度*/
int fileinfo_len = sizeof(struct fileinfo);
socklen_t sockaddr_len = sizeof(struct sockaddr);
int head_len = sizeof(struct head);

extern char *mbegin;
extern int port;


int Client_init(int port)
{
    //创建socket
    int sock_fd = socket(AF_INET,SOCK_STREAM, 0);

    //构建地址结构体
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //连接服务器
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sockaddr_len) < 0)
    {
        perror("connect");
        exit(-1);
    }
    return sock_fd;
}


void send_fileinfo(int sock_fd, char *fname, struct stat* p_fstat, struct fileinfo *p_finfo, int *p_last_bs)
{	
	/*初始化fileinfo*/
    bzero(p_finfo, fileinfo_len);
    strcpy(p_finfo->filename, fname);
    p_finfo->filesize = p_fstat->st_size;

    /*最后一个分块可能不足一个标准分块*/
    int count = p_fstat->st_size/BLOCKSIZE;
    if(p_fstat->st_size%BLOCKSIZE == 0){
        p_finfo->count = count;
    }
    else{
        p_finfo->count = count+1;
        *p_last_bs = p_fstat->st_size - BLOCKSIZE*count;
    }
    p_finfo->bs = BLOCKSIZE;

    /*发送type和文件信息*/
    char send_buf[FILEINFO_MAXLEN]= {0};
    int type=0;
    memcpy(send_buf, &type, INT_SIZE);
    memcpy(send_buf+INT_SIZE, p_finfo, fileinfo_len);
    send(sock_fd, send_buf, fileinfo_len+INT_SIZE,  0);

	printf("-------- fileinfo -------\n");
    printf("filename= %s\nfilesize= %d\ncount= %d\nblocksize= %d\n", p_finfo->filename, p_finfo->filesize, p_finfo->count, p_finfo->bs);
	printf("-------------------------\n");
    return;
}


void * send_filedata(void *args)
{
    struct head * p_fhead = (struct head *)args;
    printf("------- blockhead -------\n");
    printf("filename= %s\nThe filedata id= %d\noffset= %d\nbs= %d\n", p_fhead->filename, p_fhead->id, p_fhead->offset, p_fhead->bs);
    printf("-------------------------\n");

    int sock_fd = Client_init(port);
	//set_fd_noblock(sock_fd);

	/*发送type和数据块头部*/
    char send_buf[FILEINFO_MAXLEN]= {0};
    int type=255;
    memcpy(send_buf, &type, INT_SIZE);
	memcpy(send_buf+INT_SIZE, p_fhead, head_len);
	int sendsize=0, len = head_len+INT_SIZE;
    char *p=send_buf;
    while(1){
        if((send(sock_fd, p, 1, 0) >0)){
            ++sendsize;
            if(sendsize == len)
                break;
            ++p;
        }
    }
//	printf("head_len = %d ; send head: sendsize = %d\n",head_len, sendsize);

	/*发送数据块*/
	printf("Thread : send filedata\n");
	int i=0, send_size=0, num=p_fhead->bs/SEND_SIZE;
    unsigned int final_size = p_fhead->bs - SEND_SIZE * num;
    /*记录最后一块不满足SEND_SIZE块的大小*/
	unsigned char *fp=mbegin+p_fhead->offset;
	for(i=0; i<num; i++)
    {
		if( (send_size = send(sock_fd, fp, SEND_SIZE, 0)) == SEND_SIZE)
        {
			fp+=SEND_SIZE;
//			printf("fp = %p ; a SEND_SIZE ok\n", fp);
		}
		else
        {
//			printf("send_size = %d ;  a SEND_SIZE erro\n",send_size);
		}
	}

    /*发送最后一块不满足SEND_SIZE的数据*/
    if( (send_size = send(sock_fd, fp, final_size, 0)) == SEND_SIZE)
    {
        fp+=SEND_SIZE;
        printf("fp = %p ; a SEND_SIZE ok\n", fp);
    }
    else{
//      printf("send_size = %d ;  a SEND_SIZE erro\n",send_size);
    }


	printf("### send a fileblock ###\n");
    close(sock_fd);
	free(args);
    return NULL;
}



struct head * new_fb_head(char *filename, int freeid, int *offset)
{
    struct head * p_fhead = (struct head *)malloc(head_len);
    bzero(p_fhead, head_len);
    strcpy(p_fhead->filename, filename);
    p_fhead->id = freeid;
    p_fhead->offset = *offset;
    p_fhead->bs = BLOCKSIZE;
    *offset += BLOCKSIZE;
    return p_fhead;
}


/* 功  能：将str字符串中的oldstr字符串替换为newstr字符串
 * 参  数：str：操作目标 oldstr：被替换者 newstr：替换者
 * 返回值：返回替换之后的字符串
 * 版  本： V0.2
 */
char *strrpc(char *str,char *oldstr,char *newstr)
{
    char bstr[strlen(str)];//转换缓冲区
    memset(bstr,0,sizeof(bstr));
 
    for(int i = 0;i < strlen(str);i++){
        if(!strncmp(str+i,oldstr,strlen(oldstr))){//查找目标字符串
            strcat(bstr,newstr);
            i += strlen(oldstr) - 1;
        }else{
            strncat(bstr,str + i,1);//保存一字节进缓冲区
        }
    }
 
    strcpy(str,bstr);
    return str;
}


char *getFileName(char *filename)
{
    char *filename2 = NULL;
    char *filename3 = NULL;
    
    filename2 = strtok(filename, "/");
    //printf("%s, %s\n", filename, filename2);

    while(filename2 = strtok(NULL, "/"))
    {
        filename3 = filename2;
    }

    if(filename2 != NULL)
        strcpy(filename, filename3);

    //printf("%s, %s, %s\n", filename, filename2, filename3);
    
    if(filename3 == NULL)
    {
        //printf("%s, %s, %s\n", filename, filename2, filename3);
        //printf("%s\n", filename);
    }
    else
    {
        //printf("%s\n", filename3);
        strcpy(filename, filename3);

    }   

    return filename;
}
