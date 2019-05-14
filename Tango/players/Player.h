#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QSqlDatabase>

class Player : public QObject
{
    Q_OBJECT
public:
    Player(QSqlDatabase db_handler=QSqlDatabase::database(), QObject *parent = nullptr);

signals:

public slots:

public:
    virtual bool sign_up(QString account, QString password) = 0;
    virtual bool sign_in(QString account, QString password) = 0;
    virtual bool login_out() = 0;

    virtual bool update_full_info() = 0;
protected:
    QSqlDatabase handler;
};

#endif // PLAYER_H
