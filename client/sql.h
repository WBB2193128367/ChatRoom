void open_db(sqlite3 **db);
void create_user_db(sqlite3 *db, char *name);
void insert_retfriend_db(sqlite3 *db, Msg *Pmsg);
void insert_addfriend_db(sqlite3 *db, Msg *Pmsg);
void history_msg_person_db(sqlite3 *db, Msg *Pmsg);
void history_msg_group_db(sqlite3 *db, Msg *Pmsg);