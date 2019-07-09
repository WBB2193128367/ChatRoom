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


#define RETURNID	2000
#define LOGINOK 	2001
#define LOGINFAIL 	2002


void * server_child(void * arg);
int InitNet(void);
void handler(int);
void InitDataBase(void);
void user_reg(Msg *, int);
int login(Msg *, int);


#endif