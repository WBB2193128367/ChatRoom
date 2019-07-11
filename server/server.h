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


#define CLIENT_MAX_SIZE 100
#define MSG_MAX_SIZE 	800
#define NAMESIZE    20
#define PSWSIZE     20
#define IP 			"192.168.1.165"
#define PORT		2222


struct message
{	
	int cmd;
	int revert;
	int id;
	int fd;
	char name[NAMESIZE];
	char toname[NAMESIZE];
	char passwd[PSWSIZE];
	char msg[MSG_MAX_SIZE];
};

typedef struct message Msg;

enum cmd
{
	REG = 1000,
	LOGIN,
	SHOWONLINE,
	EXIT,
	LOGOUT,
	CHATTO
};


enum revert
{
	RETURNID = 2000,
	LOGINOK,
	LOGINFAIL,
	ONLINEIN,
	ONLINEOUT,
	CHATOK
};

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
void handler(int);
void InitDataBase(void);
void user_reg(Msg *, int);
void login(Msg *, int);
void showOnlineFriend(int);
void exit_client(int);
void logout(int);
void chat_to(Msg *, int);




#endif