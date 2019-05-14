#ifndef AUTHOR_H
#define AUTHOR_H

#include "Player.h"


class QSqlError;

class Author : public Player
{
    Q_OBJECT
public:
    Author(QSqlDatabase db_handler, QObject *parent = nullptr);
    ~Author();

    static bool create_table();

    bool sign_up(QString account, QString password);
    bool sign_in(QString account, QString password);
    bool login_out();

    bool update_full_info();

    const QString last_error();

    int user_id;
    QString name;

    int tango_count;
    int exp;

    int misson_count;
    int level;

    QString motto;

private:
    QString _last_error;
};

#endif // AUTHOR_H
