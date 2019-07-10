#ifndef _CLIENT_H
#define _CLIENT_H


#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include "debug.h"



#define MSG_MAX_SIZE 	1024
#define NAMESIZE    20
#define PSWSIZE     20
#define IP 			"192.168.1.165"
#define PORT		2222


struct message
{	
	int cmd;
	char msg[MSG_MAX_SIZE];
};

typedef struct message Msg;

#define REG		1000
#define LOGIN	1001
#define SHOWONLINE	1002
#define EXIT 	1003
#define LOGOUT	1004


#define RETURNID	2000
#define LOGINOK		2001
#define LOGINFAIL 	2002

struct list
{
	int fd;
	int id;
	char name[20];

	struct list *next;
};

typedef struct list Node;
typedef struct list * Link;


int InitNet(void);
void main_handler(int);
void user_reg(int);
void login(char *, int);
void is_malloc_ok(Link);
void create_node(Link * newnode);
void insert_head(Link * head, Link newnode);
void display_list(Link head);
void delete_node(Link * head, int cfd);
void showOnlineFriend(int);
void exit_client(int);
void logout(int);
void is_send_recv_ok(int, char*);


#endif