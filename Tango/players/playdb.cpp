
#include "playdb.h"


PlayerDB::PlayerDB(QSqlDatabase db)
{
    db_handler = db;
}

PlayerDB::PlayerDB(PlayerDB &right_db)
{
    db_handler = right_db.db_handler;
}

PlayerDB::~PlayerDB(){}
