
#include <QTcpSocket>
#include <QSqlError>
#include <QSqlQuery>

#include "Client.h"
#include "../players/Author.h"
#include "../types/TangoPair.h"

Client::Client(QObject *parent) : QObject(parent)
{
    this->user_author = nullptr;

    this->remote_server = new QTcpSocket;

    local_handler = QSqlDatabase::addDatabase("QMYSQL");
    local_handler.setHostName("localhost");
    local_handler.setDatabaseName("tango");
    local_handler.setUserName("tangosql");
    local_handler.setPassword("123456");

    this->remote_ready = false;
    this->make_remote_server_on_connected();
    this->make_remote_server_on_disconnected();

    this->local_ready = false;
}

Client::~Client()
{
    qDebug() << "release client";
    this->disconnect_to_local();
    this->disconnect_to_remote();
}


bool Client::setup_remote_connection(QHostAddress host_address, quint16 server_port)
{
    if (this->remote_ready == false) {
        remote_server->connectToHost(host_address, server_port);
        this->remote_address = host_address;
        this->remote_port = server_port;
        goto wait_for_connection;
    }

    if (this->remote_address != host_address || this->remote_port != server_port) {
        this->disconnect_to_remote();
        remote_server->connectToHost(host_address, server_port);
        this->remote_address = host_address;
        this->remote_port = server_port;
        goto wait_for_connection;
    }

    qDebug() << "might be open" << this->remote_server->isOpen();
    this->switch_remote_mode();
    return true;

wait_for_connection:
    if (this->remote_server->waitForConnected(1000) == false) {
        _last_error = "服务器连接失败";
        return false;
    }

    qDebug() << "might be open" << this->remote_server->isOpen();
    this->switch_remote_mode();
    return true;
}


bool Client::setup_local_connection()
{
    if (this->local_ready == false) {
        if (!local_handler.open()) {
            qDebug() << "db open error:" << local_handler.lastError().text();
            _last_error = local_handler.lastError().text();
            return false;
        }
        if (!this->create_tables()) {
            return false;
        }
        this->local_ready = true;
        this->switch_local_mode();
        return true;
    }
    this->switch_local_mode();
    return true;
}


bool Client::disconnect_to_remote()
{
    qDebug() << "is open?" << this->remote_server->isOpen();
    if (this->remote_ready) {
        this->remote_ready = false;
        remote_server->disconnectFromHost();
        return true;
    }
    return true;
}

bool Client::disconnect_to_local()
{
    if (this->local_ready) {
        local_handler.close();
        return true;
    }
    return true;
}

bool Client::is_remote_server_connected()
{
    return this->remote_ready;
}

bool Client::is_local_handler_connected()
{
    return this->local_ready;
}


inline void Client::make_remote_server_on_connected()
{
    connect(this->remote_server, &QTcpSocket::connected, [this]() mutable {
        qDebug() << "remote client on connected";
        this->remote_ready = true;
        emit this->connected();
    });
}

inline void Client::make_remote_server_on_disconnected()
{
    connect(this->remote_server, &QTcpSocket::disconnected, [this]() mutable {
        qDebug() << "remote client on disconnected";
        this->remote_ready = false;
        emit this->disconnected();
    });
}

bool Client::author_sign_in_local(QString account, QString password)
{
    if (this->user_status != Client::UserStatusType::None) {
        qDebug() << "user_doesn't logout";
        user_author->deleteLater();
        user_author = nullptr;
        this->user_status = Client::UserStatusType::None;
    }

    user_author = new class Author(QSqlDatabase::database());
    if (user_author->sign_in_local(account, password)) {
        _last_error = user_author->last_error();
        this->user_status = Client::Author;
        return true;
    }

    _last_error = user_author->last_error();
    user_author->deleteLater();
    user_author = nullptr;
    return false;
}


bool Client::author_sign_up_local(QString account, QString password)
{
    if (this->user_status != Client::UserStatusType::None) {
        qDebug() << "user_doesn't logout";
        user_author->deleteLater();
        user_author = nullptr;
        this->user_status = Client::UserStatusType::None;
    }

    user_author = new class Author(QSqlDatabase::database());
    if (user_author->sign_up_local(account, password)) {
        _last_error = user_author->last_error();
        this->user_status = Client::Author;
        return true;
    }

    _last_error = user_author->last_error();
    user_author->deleteLater();
    user_author = nullptr;
    return false;
}


bool Client::author_sign_in_remote(QString account, QString password)
{
    _last_error = "TODO";
    QByteArray qbytes;
    qbytes.append(account);
    this->remote_server->write(qbytes);

    qbytes.clear();
    qbytes.append(password);
    this->remote_server->write(qbytes);

    return false;
}

bool Client::author_sign_up_remote(QString account, QString password)
{
    _last_error = "TODO";

    QByteArray qbytes;
    qbytes.append(account);
    this->remote_server->write(qbytes);

    qbytes.clear();
    qbytes.append(password);
    this->remote_server->write(qbytes);
    return false;
}

bool Client::submit_tango_items_local(const std::vector<TangoPair> &tango_list)
{
    qDebug() << "tango list" << tango_list;
    _last_error = "TODO";

    return false;
}

bool Client::submit_tango_items_remote(const std::vector<TangoPair> &tango_list)
{
    qDebug() << "tango list" << tango_list;
    _last_error = "TODO";
    return false;
}


std::function<void()> Client::switch_remote_mode_slottor()
{
    return [this]() mutable {
        this->author_sign_in = [this](QString account, QString password) mutable {
            return this->author_sign_in_remote(account, password);
        };
        this->author_sign_up = [this](QString account, QString password) mutable {
            return this->author_sign_up_remote(account, password);
        };
        this->submit_tango_items = [this](const std::vector<TangoPair> &tango_list) mutable {
            return this->submit_tango_items_remote(tango_list);
        };
    };
}

void Client::switch_remote_mode()
{
    this->switch_remote_mode_slottor()();
}


std::function<void()> Client::switch_local_mode_slottor()
{
    return [this]() mutable {
        this->author_sign_in = [this](QString account, QString password) mutable {
            return this->author_sign_in_local(account, password);
        };
        this->author_sign_up = [this](QString account, QString password) mutable {
            return this->author_sign_up_local(account, password);
        };
        this->submit_tango_items = [this](const std::vector<TangoPair> &tango_list) mutable {
            return this->submit_tango_items_local(tango_list);
        };
    };
}

const QString Client::last_error()
{
    return this->_last_error;
}

void Client::switch_local_mode()
{
    this->switch_local_mode_slottor()();
}


bool Client::create_author_table()
{
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
    QSqlQuery query(this->local_handler);
    query.exec("set names 'utf8'");

    if (!query.exec(create_command)) {
        qDebug() << "create authors table failed" << query.lastError().text();
        _last_error = query.lastError().text();
        return false;
    }
    return true;
}

inline bool Client::create_tangos_table()
{
    static const char *create_command =
        "create table if not exists `tangos` ("
        "    `id` int(11) NOT NULL AUTO_INCREMENT,"
        "    `key` varchar(50) NOT NULL,"
        "    `value` longtext NOT NULL,"
        "    PRIMARY KEY (`id`),"
        "    UNIQUE KEY `key_UNIQUE` (`key`),"
        "    UNIQUE KEY `id_UNIQUE` (`id`)"
        " ) engine=InnoDB default charset=utf8 comment='tango list'";
    QSqlQuery query(this->local_handler);
    query.exec("set names 'utf8'");

    if (!query.exec(create_command)) {
        qDebug() << "create authors table failed" << query.lastError().text();
        _last_error = query.lastError().text();
        return false;
    }
    return true;
}


bool Client::create_tables()
{
    if (!this->create_author_table()) {
        return false;
    }
    if (!this->create_author_table()) {
        return false;
    }
    return true;
}