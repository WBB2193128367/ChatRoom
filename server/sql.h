
void is_sqlite_ok(int ret, sqlite3 *db);
int reg_db(Msg *);
void log_db(Msg *);
void updata_passwd(Msg *Pmsg);
void find_friend(Msg *Pmsg);
void toid_to_toname(Msg *Pmsg);
void add_friend_db(Msg *Pmsg);
void offline_msg_db(Msg *Pmsg);
void find_user_db(Msg *Pmsg);
void get_offline_db(int fd, Msg *Pmsg);
void create_group_db(Msg *Pmsg);
void show_group_db(Msg *Pmsg);
void join_group_db(Msg *Pmsg);
void chat_group_db(Msg *Pmsg);