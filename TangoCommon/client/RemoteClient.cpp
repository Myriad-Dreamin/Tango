#include "RemoteClient.h"

#include <random>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include "json_rpc.h"
#include <QTcpSocket>

#include "../players/Author.h"
#include "../players/Consumer.h"
#include "../types/TangoPair.h"
#include "../types/UserBriefInfo.h"
#include "../types/UserFullInfo.h"
#include "../types/UserStatus.h"
#include "GameAutomation.h"
#include "GameConfig.h"
#include "../network/SocketX.h"

RemoteClient::RemoteClient(QObject *parent) : QObject(parent)
{
    this->user_author = new class Author(this);
    this->user_consumer = new class Consumer(this);
    this->user_status = UserStatus::None;

    this->handler = new SocketX(qintptr(1LL), this);
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

const UserFullInfo &RemoteClient::consumer_info()
{
    return this->user_consumer->user_info;
}

const UserFullInfo &RemoteClient::author_info()
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
    qDebug() << "conne" << this->handler->socketDescriptor();
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
    this->handler->write_package(client_rpc::author_sign_in_request(account, password));

    _last_error = "timeout";
    bool success = false;
    QJsonValue ret;
    int id;
    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
        qDebug() << "ret" << bytes_json;
        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
    }, 3000);
    qDebug() << ret;
    if (success == false) {
        return false;
    }
    this->user_author->user_info = UserFullInfo::from_json_array(ret.toArray());
    user_status_util::add_author_status(this->user_status);
    return success;
}

bool RemoteClient::author_sign_up(QString account, QString password)
{
    this->handler->write_package(client_rpc::author_sign_up_request(account, password));

    _last_error = "timeout";
    bool success = false;
    QJsonValue ret;
    int id;
    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
        qDebug() << "ret" << bytes_json;
        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
    }, 3000);
    qDebug() << ret;
    if (success == false) {
        return false;
    }
    this->user_author->user_info = UserFullInfo::from_json_array(ret.toArray());
    user_status_util::add_author_status(this->user_status);

    return success;
}

bool RemoteClient::consumer_sign_in(QString account, QString password)
{
    this->handler->write_package(client_rpc::consumer_sign_in_request(account, password));

    _last_error = "timeout";
    bool success = false;
    QJsonValue ret;
    int id;
    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
        qDebug() << "ret" << bytes_json;
        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
    }, 3000);
    qDebug() << ret;
    if (success == false) {
        return false;
    }
    this->user_consumer->user_info = UserFullInfo::from_json_array(ret.toArray());
    user_status_util::add_consumer_status(this->user_status);

    return success;
}

bool RemoteClient::consumer_sign_up(QString account, QString password)
{
    this->handler->write_package(client_rpc::consumer_sign_up_request(account, password));

    _last_error = "timeout";
    bool success = false;
    QJsonValue ret;
    int id;
    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
        qDebug() << "ret" << bytes_json;
        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
    }, 3000);
    qDebug() << ret;
    if (success == false) {
        return false;
    }
    this->user_consumer->user_info = UserFullInfo::from_json_array(ret.toArray());
    user_status_util::add_consumer_status(this->user_status);

    return success;
}


bool RemoteClient::logout()
{
    this->handler->write_package(client_rpc::logout_requset());
    _last_error = "timeout";
    bool success = false;
    QJsonValue ret;
    int id;
    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
        qDebug() << "ret" << bytes_json;
        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
    }, 3000);
    qDebug() << ret;
    if (success == false) {
        return false;
    }
    this->user_status = UserStatus(static_cast<uint8_t>(ret.toInt()));

    return success;
}

bool RemoteClient::sync_status()
{
    this->handler->write_package(client_rpc::sync_status_request());
    _last_error = "timeout";
    bool success = false;
    QJsonValue ret;
    int id;
    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
        qDebug() << "ret" << bytes_json;
        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
    }, 3000);
    qDebug() << ret;
    if (success == false) {
        return false;
    }
    QJsonArray arr = ret.toArray();
    this->user_status = UserStatus(static_cast<uint8_t>(arr[0].toInt()));
    this->user_author->user_info = UserFullInfo::from_json_array(arr, 1);
    this->user_consumer->user_info = UserFullInfo::from_json_array(arr, 8);

    return false;
}

int RemoteClient::consumer_exp()
{
    return this->user_consumer->user_info.exp;
}

int RemoteClient::consumer_level()
{
    return this->user_consumer->user_info.level;
}

bool RemoteClient::consumer_logining()
{
    return user_status_util::has_consumer_status(this->user_status);
}

bool RemoteClient::author_logining()
{
    return user_status_util::has_author_status(this->user_status);
}

bool RemoteClient::submit_tango_items(const std::vector<TangoPair> &tango_list)
{
    this->handler->write_package(client_rpc::submit_tango_items_request(tango_list));
    _last_error = "timeout";
    bool success = false;
    QJsonValue ret;
    int id;
    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
        qDebug() << "ret" << bytes_json;
        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
    }, 3000);
    qDebug() << ret;
    if (success == false) {
        return false;
    }
    this->user_author->user_info = UserFullInfo::from_json_array(ret.toArray());
    return success;
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
