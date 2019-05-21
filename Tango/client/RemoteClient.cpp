#include "RemoteClient.h"

#include <random>
#include "../players/Author.h"
#include "../players/Consumer.h"
#include "../types/TangoPair.h"
#include "../types/UserBriefInfo.h"
#include "../types/UserFullInfo.h"
#include "../types/UserStatus.h"
#include "GameAutomation.h"
#include "GameConfig.h"

#include <QTcpSocket>

RemoteClient::RemoteClient(QObject *parent) : QObject(parent)
{
    this->user_author = nullptr;
    this->user_consumer = nullptr;
    this->user_status = UserStatus::None;

    this->handler = new QTcpSocket;
    this->ready = false;


    this->make_server_on_connected();
    this->make_server_on_disconnected();

}
RemoteClient::~RemoteClient()
{

}

bool RemoteClient::is_connected()
{
    return this->ready;
}

const UserFullInfo RemoteClient::consumer_info()
{
    return this->user_consumer->user_info;
}

const UserFullInfo RemoteClient::author_info()
{
    return this->user_author->user_info;
}

bool RemoteClient::setup_connection(QHostAddress host_address, quint16 server_port)
{
    if (this->ready == false) {
        handler->connectToHost(host_address, server_port);
        this->remote_address = host_address;
        this->remote_port = server_port;
        goto wait_for_connection;
    }

    if (this->remote_address != host_address || this->remote_port != server_port) {
        this->stop_connection();
        handler->connectToHost(host_address, server_port);
        this->remote_address = host_address;
        this->remote_port = server_port;
        goto wait_for_connection;
    }

    qDebug() << "might be open" << this->handler->isOpen();
    return true;

wait_for_connection:
    if (this->handler->waitForConnected(1000) == false) {
        _last_error = "服务器连接失败";
        return false;
    }

    qDebug() << "might be open" << this->handler->isOpen();
    return true;
}

bool RemoteClient::stop_connection()
{
    qDebug() << "is open?" << this->handler->isOpen();

    if (this->ready) {
        this->ready = false;
        handler->disconnectFromHost();
        handler = nullptr;
        return true;
    }

    return true;
}

inline void RemoteClient::make_server_on_connected()
{
    connect(this->handler, &QTcpSocket::connected, [this]() mutable {
        qDebug() << "remote client on connected";
        this->ready = true;
        emit this->connected();
    });
}

inline void RemoteClient::make_server_on_disconnected()
{
    connect(this->handler, &QTcpSocket::disconnected, [this]() mutable {
        qDebug() << "remote client on disconnected";
        this->ready = false;
        emit this->disconnected();
    });
}

bool RemoteClient::author_sign_in(QString account, QString password)
{
    _last_error = "TODO";
    QByteArray qbytes;
    qbytes.append(account);
    qbytes.append(password);
    this->handler->write(qbytes);

    qbytes.clear();
    this->handler->write(qbytes);

    return false;
}

bool RemoteClient::author_sign_up(QString account, QString password)
{
    _last_error = "TODO";

    QByteArray qbytes;
    qbytes.append(account);
    this->handler->write(qbytes);

    qbytes.clear();
    qbytes.append(password);
    this->handler->write(qbytes);

    return false;
}

bool RemoteClient::consumer_sign_in(QString account, QString password)
{
    _last_error = "TODO";

    QByteArray qbytes;
    qbytes.append(account);
    this->handler->write(qbytes);

    qbytes.clear();
    qbytes.append(password);
    this->handler->write(qbytes);

    return false;
}

bool RemoteClient::consumer_sign_up(QString account, QString password)
{
    _last_error = "TODO";

    QByteArray qbytes;
    qbytes.append(account);
    this->handler->write(qbytes);

    qbytes.clear();
    qbytes.append(password);
    this->handler->write(qbytes);

    return false;
}


bool RemoteClient::logout()
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::sync_status()
{
    _last_error = "TODO";
    return false;
}

int RemoteClient::consumer_exp()
{
    _last_error = "TODO";
    return 0;
}

int RemoteClient::consumer_level()
{
    _last_error = "TODO";
    return 0;
}

bool RemoteClient::consumer_logining()
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::author_logining()
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::submit_tango_items(const std::vector<TangoPair> &tango_list)
{
    _last_error = "TODO";
    return false;
}

AbstractGameAutomation *RemoteClient::start_game_event(const GameConfig *game_config, int n, RetriveMode mode)
{
    _last_error = "TODO";
    return nullptr;
}

bool RemoteClient::settle_game_event(const AbstractGameAutomation *automate)
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::query_authors_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::query_consumers_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::query_authors_by_id(UserFullInfo &query_container, int id)
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::query_consumers_by_id(UserFullInfo &query_container, int id)
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::query_consumers_by_name(UserFullInfo &query_container, QString name)
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::query_users(int &query_count)
{
    _last_error = "TODO";
    return false;
}

bool RemoteClient::query_authors_by_name(UserFullInfo &query_container, QString name)
{
    _last_error = "TODO";
    return false;
}

const QString RemoteClient::last_error()
{
    return _last_error;
}
