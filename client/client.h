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
#include <sqlite3.h>



#define MSG_MAX_SIZE 	1024
#define NAMESIZE    20
#define PSWSIZE     20
#define IP 			"192.168.1.165"
#define PORT		2222

#define PORT_END	4000
#define PORT_START	3000



struct message
{	
	int cmd;
	int revert;
	int id;
	int toid;
	int group_id;
	int fd;
	int flag;
	char name[NAMESIZE];
	char group[NAMESIZE];
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
	CHATTO,
	ADDFRIEND,
	RETFRIEND,
	PASSWD,
	CHATALL,
	CHATTOID,
	GETOFFLINE,
	SHOWGROUP,
	CREATGROUP,
	JOINGROUP,
	CHATGROUP,
	LOGINNAME,
	QUITGROUP,
	SHOWGROUPMEMBER,
	SENDFILE
};


enum revert
{
	RETURNID = 2000,
	LOGINOK,
	LOGINFAIL,
	ONLINEIN,
	ONLINEOUT,
	CHATOK,
	READD,
	EXIST,
	NOEXIST,
	ADDFRIENDOK,
	RETFRIENDOK,
	SENDLOGIN,
	SENDLOGOUT,
	OFFLINE,
	GROUPOK,
	GROUPFAIL,
	GROUPEXIST,
	GROUPNOTEXIST,
	JOINSUCCESS,
	QUITOK,
	RETPORT
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
void login_name(char *name, int fd);
void exit_client(int);
void logout(int, char *);


#endif