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
	int toid;
	int group_id;
	int fd;
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
	CAHTTOID,
	GETOFFLINE,
	SHOWGROUP,
	CREATGROUP,
	JOINGROUP,
	CHATGROUP
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
	JOINSUCCESS
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
void sendall_login(int fd, Msg *Pmsg);
void showOnlineFriend(int);
void add_friend(int, Msg *);
void ret_friend(int fd, Msg *Pmsg);
void passwd(int, Msg *);
void exit_client(int);
void logout(int, Msg *Pmsg);
void chat_to(Msg *, int);
void chat_all(int fd, Msg *Pmsg);
void chat_to_id(int fd, Msg *Pmsg);
void get_offline_msg(int fd, Msg *Pmsg);
void create_group(int fd, Msg *Pmsg);
void show_group(int fd, Msg *Pmsg);
void join_group(int fd, Msg *Pmsg);
void chat_group(int fd, Msg *Pmsg);


#endif