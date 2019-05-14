
#include "Author.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>



Author::Author(QSqlDatabase db_handler, QObject *parent): Player(db_handler, parent)
{
    _last_error = nullptr;
}

Author::~Author()
{
}


bool Author::create_table() {
    static const char *create_command =
        "create table if not exists `authors` ("
        "    `id` int(11) NOT NULL AUTO_INCREMENT,"
        "    `name` varchar(45) NOT NULL,"
        "    `password` varchar(45) NOT NULL,"
        "    `exp` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `level` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `tango_count` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `misson_count` int(10) unsigned NOT NULL DEFAULT '0',"
        "    `motto` longtext,"
        "     PRIMARY KEY (`id`),"
        "     UNIQUE KEY `name_UNIQUE` (`name`)"
        " ) engine=InnoDB default charset=utf8 comment='authors that produce good tango list for users'";
    QSqlQuery query(QSqlDatabase::database());
    query.exec("set names 'utf8'");

    if (!query.exec(create_command)) {
        qDebug() << "create authors table failed" << query.lastError().text();
        return false;
    }
    return true;
}

bool Author::sign_up(QString account, QString password)
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

bool Author::sign_in(QString account, QString password)
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

    this->user_id = query.value(0).toInt();
    this->name = query.value(1).toString();
    this->exp = query.value(3).toInt();
    this->level = query.value(4).toInt();
    this->tango_count = query.value(5).toInt();
    this->misson_count = query.value(6).toInt();
    this->motto = query.value(7).toString();

    return true;
}

bool Author::login_out()
{
    if (!this->update_full_info()) {
        return false;
    }

    return true;
}

bool Author::update_full_info()
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
    query.bindValue(":id", this->user_id);
    query.bindValue(":exp", this->exp);
    query.bindValue(":level", this->level);
    query.bindValue(":tangocount", this->tango_count);
    query.bindValue(":missoncount", this->misson_count);

    if (query.exec() == false) {
        _last_error = query.lastError().text();
        return false;
    }

    return true;
}

const QString Author::last_error()
{
    return this->_last_error;
}


