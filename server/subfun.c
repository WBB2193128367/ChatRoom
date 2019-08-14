#include "server.h"
#include "package.h"

/* 功  能：将str字符串中的oldstr字符串替换为newstr字符串
 * 参  数：str：操作目标 oldstr：被替换者 newstr：替换者
 * 返回值：返回替换之后的字符串
 * 版  本： V0.2
 */
char *strrpc(char *str,char *oldstr,char *newstr){
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

/*
    功能：随机生成一个PORT，从PORT_START到PORT_END中间随机一个端口号
    返回值：i_random_port：端口号
 */
int get_port()
{
	int i_random_port;
    time_t t;
    char psz_port_cmd[MSG_MAX_SIZE];
    srand((unsigned)time(&t));
    do
    {
        i_random_port = rand() % (PORT_END - PORT_START + 1) + PORT_START;
        sprintf(psz_port_cmd, "netstat -an | grep :%d > /dev/null", i_random_port);
    }while(!system(psz_port_cmd));
    
    printf("port=%d\n", i_random_port);
    return i_random_port;
}