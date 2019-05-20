#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QSqlDatabase>

class Player : public QObject
{
    Q_OBJECT

public:
    Player(QSqlDatabase db_handler=QSqlDatabase::database(), QObject *parent = nullptr);

public:
    virtual bool sign_up_local(QString account, QString password) = 0;
    virtual bool sign_up_remote(QString account, QString password) = 0;

    virtual bool sign_in_local(QString account, QString password) = 0;
    virtual bool sign_in_remote(QString account, QString password) = 0;

    virtual bool login_out_local() = 0;
    virtual bool login_out_remote() = 0;

    virtual bool update_full_info_local() = 0;
    virtual bool update_full_info_remote() = 0;

protected:
    QSqlDatabase handler;
};

#endif // PLAYER_H
