void process_chat(int, char *);
void process_addFriend(int, char *);
void process_showOnline(int);
void process_passwd(int);
void process_retFriend(int fd, char *name);
void process_chatall(int fd, char *name);
void process_chattoid(int fd, char *name);
void process_get_offline(int fd);

void process_showGroup(int fd);
void process_joinGroup(int fd);
void process_creatGroup(int fd);
void process_chatGroup(int fd);