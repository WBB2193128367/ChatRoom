#ifndef _SERVER_H
#define _SERVER_H


#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <sqlite3.h>
#include <fcntl.h>
#include <unistd.h>
#include "debug.h"


#define CLIENT_MAX_SIZE 100
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
#define LOGINOK 	2001
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


void * server_child(void * arg);
int InitNet(void);
void handler(int);
void InitDataBase(void);
void user_reg(Msg *, int);
int login(Msg *, int);
void is_malloc_ok(Link);
void create_node(Link * newnode);
void insert_head(Link * head, Link newnode);
void display_list(Link head);
void delete_node(Link * head, int fd);
void showOnlineFriend(int);
void exit_client(int);
void logout(int);
void is_send_recv_ok(int, char*);




#endif