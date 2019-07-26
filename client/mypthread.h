void * read_test(void *);

void read_showOnline(int fd);//上线后第一次获取在线用户的读操作
void read_showGroup(int fd, Msg *Pmsg);//获取当前用户加入的群
void printf_msg(Msg *Pmsg);
void printf_groupMember(Msg *Pmsg);//打印群用户