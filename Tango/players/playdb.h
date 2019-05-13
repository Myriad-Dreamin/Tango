
#ifndef PLAYERDB_H
#define PLAYERDB_H

#include <QSqlDatabase>
#include "../db/db_connection.h"

class PlayerDB
{
protected:
    QSqlDatabase db_handler;
public:
    PlayerDB(QSqlDatabase db=QSqlDatabase::database());

    PlayerDB(PlayerDB &right_db);
    ~PlayerDB();
};


#endif
