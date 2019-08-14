void process_chat(int, char *); //用户名私聊
void process_addFriend(int, char *);    //添加好友 
void process_showOnline(int);           //显示在线用户
void process_passwd(int);               //修改密码
void process_retFriend(int fd, char *name);     //回应好友请求
void process_chatall(int fd, char *name);       //群发
void process_chattoid(int fd, char *name);      //ID私聊
void process_get_offline(int fd);       //获取离线消息

void process_showGroup(int fd);         //显示自己加入的群
void process_joinGroup(int fd);         //加入群组
void process_creatGroup(int fd);        //创建群组
void process_chatGroup(int fd);         //群内聊天
void process_quitGroup(int fd);         //退出群组
void process_showGroupMember(int fd);   //显示群成员

void process_sendFile(int fd);          //发送文件