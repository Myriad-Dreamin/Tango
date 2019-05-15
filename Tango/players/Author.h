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

    bool sign_in_local(QString account, QString password);
    bool sign_in_remote(QString account, QString password);

    bool sign_up_local(QString account, QString password);
    bool sign_up_remote(QString account, QString password);

    bool login_out_local();
    bool login_out_remote();

    bool update_full_info_local();
    bool update_full_info_remote();

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
