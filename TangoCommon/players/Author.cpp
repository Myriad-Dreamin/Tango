
#include "Author.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>


Author::Author(QSqlDatabase db_handler, QObject *parent): Player(db_handler, parent)
{
    _last_error = nullptr;

}

Author::Author(QObject *parent): Player(parent)
{
    _last_error = nullptr;

}


Author::~Author()
{
}

bool Author::sign_up_local(QString account, QString password)
{
    qDebug() << "account: " << account << "password: " << password;
    static const char *sign_up_command = "insert into `authors` (name, password) VALUES (:Name, :Password)";
    QSqlQuery query(this->handler);
    query.prepare(sign_up_command);
    query.bindValue(":Name", account);
    query.bindValue(":Password", password);

    if (query.exec() == false) {
        qDebug() << query.executedQuery();
        _last_error = query.lastError().text();
        return false;
    }

    return true;
}

bool Author::sign_up_remote(QString account, QString password)
{
    _last_error = "TODO";
    qDebug() << "account: " << account << "password: " << password;

    return true;
}

bool Author::sign_in_local(QString account, QString password)
{
    qDebug() << "account: " << account << "password: " << password;
    static const char *sign_in_command = "select * from `authors` where name = :name";

    QSqlQuery query(this->handler);
    query.prepare(sign_in_command);
    query.bindValue(":name", account);

    if (query.exec() == false) {
        _last_error = query.lastError().text();
        return false;
    }

    qDebug() << query.executedQuery();

    if(!query.first()) {
        _last_error = "account not found";
        return false;
    }

    if (query.value(2).toString() != password) {
        qDebug() << query.value(2).toString() << " " << password;
        _last_error = "账户密码错误";
        return false;
    }

    this->user_info.user_id = query.value(0).toInt();
    this->user_info.name = query.value(1).toString();
    this->user_info.exp = query.value(3).toInt();
    this->user_info.level = query.value(4).toInt();
    this->user_info.tango_count = query.value(5).toInt();
    this->user_info.misson_count = query.value(6).toInt();
    this->user_info.motto = query.value(7).toString();

    return true;
}

bool Author::sign_in_remote(QString account, QString password)
{
    _last_error = "TODO";
    qDebug() << "account: " << account << "password: " << password;

    return false;
}

bool Author::login_out_local()
{
    if (!this->update_full_info_local()) {
        return false;
    }

    return true;
}

bool Author::login_out_remote()
{
    _last_error = "TODO";

    return false;
}

bool Author::update_full_info_local()
{
    static const char *update_command =
        "update `authors` set "
        "    `exp` = :exp,"
        "    `level` = :level,"
        "    `tango_count` = :tangocount,"
        "    `misson_count` = :missoncount "
        "where id = :id";

    QSqlQuery query(this->handler);
    query.prepare(update_command);
    query.bindValue(":id", this->user_info.user_id);
    query.bindValue(":exp", this->user_info.exp);
    query.bindValue(":level", this->user_info.level);
    query.bindValue(":tangocount", this->user_info.tango_count);
    query.bindValue(":missoncount", this->user_info.misson_count);

    if (query.exec() == false) {
        _last_error = query.lastError().text();
        return false;
    }

    return true;
}

bool Author::update_full_info_remote()
{
    _last_error = "TODO";

    return false;
}

const QString Author::last_error()
{
    return this->_last_error;
}


