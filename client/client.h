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
#include <pthread.h>



#define MSG_MAX_SIZE 	1024
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



void main_handler(int);
void user_reg(int);
void login(char *, int);
void process_showOnline(int);
void exit_client(int);
void logout(int);


#endif