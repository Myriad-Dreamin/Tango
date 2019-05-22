#include "RemoteClient.h"

#include <QLabel>
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
#include "GameAutomationRelayer.h"
#include "GameConfig.h"
#include "scene/MainScene.h"
#include "scene/PlayingScene.h"
#include "scene/PlaySubScene.h"
#include "scene/RegisterScene.h"
#include "scene/CreationScene.h"
#include "scene/SelectingScene.h"
#include "scene/PlaySettleScene.h"
#include "scene/RankingAuthorsScene.h"
#include "scene/RankingConsumersScene.h"
#include "scene/QueryUsersScene.h"
#include "../types/MessageBox.h"
#include "../network/SocketX.h"

RemoteClient::RemoteClient(QObject *parent) : QObject(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);

    this->user_author = new class Author(this);
    this->user_consumer = new class Consumer(this);
    this->user_status = UserStatus::None;

    this->handler = new SocketX(qintptr(1LL), this);
    this->ready = false;

    connect(handler, &SocketX::package_ready, this, &RemoteClient::receive_packages);


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


void RemoteClient::receive_packages(const QString &ip, const quint16 &port, const QByteArray &data)
{
    qDebug() << "package received" << ip << port << data;

    QString err;
    QJsonValue params;
    int id;bool para;
    if (client_rpc::decode_json_object(data, params, para, id, err)) {
        qDebug() << "ww" << err;
        if (para) {
            this->params_packages(id, params.toArray());
        } else {
            this->returns_packages(id, params, err);
        }
        return;
    }
    qDebug() << "errobj" << err;
    return;
}

void RemoteClient::params_packages(int id, QJsonArray params)
{
    switch(id) {
    case client_rpc::signal_start_game: {
        emit game_signal_start_game();
        return;
    }
    case client_rpc::signal_new_tango: {
        qDebug() << "new tango" << params;
        if (params.size() != 2) {
            // ignore
            qDebug() << "err signal new tango" << params;
            return;
        }
        if (!params[0].isArray() || (params[1].toInt(2133333333) == 2133333333)) {
            // ignore
            qDebug() << "err signal new tango" << params;
            return;
        }
        TangoPair tango = TangoPair::from_json_array(params[0].toArray());
        current_key = tango.first;
        emit game_signal_new_tango(tango, params[1].toInt());
        return;
    }
    case client_rpc::signal_tango_faded: {
        if (params.size() != 1) {
            // ignore
            qDebug() << "err signal tango faded" << params;
            return;
        }
        int answer_time = params[0].toInt(-1);
        if (answer_time < 0) {
            // ignore
            qDebug() << "err signal tango faded" << params;
            return;
        }
        emit game_signal_tango_faded(answer_time);
        return;
    }
    case client_rpc::signal_game_success: {
        qDebug() << "game success received";
        emit game_signal_success();
        return;
    }
    case client_rpc::signal_game_failed: {
        qDebug() << "game success failed";
        emit game_signal_failed();
        return;
    }
    default:
        // ignore
        qDebug() << "err signal tango faded" << params;
        return;
    }
}

void RemoteClient::returns_packages(int id, QJsonValue rets, QString err)
{
    qDebug() << id << rets << err;
    switch (id) {

    case client_rpc::author_sign_in: {
        if (err != nullptr) {
            this->parent->switch_scene(this->parent->main_scene);
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        this->user_author->user_info = UserFullInfo::from_json_array(rets.toArray());
        user_status_util::add_author_status(this->user_status);
        this->parent->selecting_scene->set_visble_buttons();
        return;
    }
    case client_rpc::author_sign_up: {
        if (err != nullptr) {
            this->parent->switch_scene(this->parent->register_scene);
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        this->user_author->user_info = UserFullInfo::from_json_array(rets.toArray());
        user_status_util::add_author_status(this->user_status);
        this->parent->selecting_scene->set_visble_buttons();
        return;
    }
    case client_rpc::consumer_sign_in: {
        if (err != nullptr) {
            this->parent->switch_scene(this->parent->main_scene);
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        this->user_consumer->user_info = UserFullInfo::from_json_array(rets.toArray());
        user_status_util::add_consumer_status(this->user_status);
        this->parent->selecting_scene->set_visble_buttons();
        return;
    }
    case client_rpc::consumer_sign_up: {
        if (err != nullptr) {
            this->parent->switch_scene(this->parent->register_scene);
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        this->user_consumer->user_info = UserFullInfo::from_json_array(rets.toArray());
        user_status_util::add_consumer_status(this->user_status);
        this->parent->selecting_scene->set_visble_buttons();
        return;
    }
    case client_rpc::logout: {
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        this->user_status = UserStatus(static_cast<uint8_t>(rets.toInt()));
        return;
    }
    case client_rpc::submit_tango_items: {
        if (err != nullptr) {
            this->parent->switch_scene(this->parent->creation_scene);
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        this->user_author->user_info = UserFullInfo::from_json_array(rets.toArray());
        return;
    }
    case client_rpc::settle_game_event: {
        qDebug() << "settle gaming ...!!!!";
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        this->user_consumer->user_info = UserFullInfo::from_json_array(rets.toArray());
        this->parent->playset_scene->to_exp->setNum(this->user_consumer->user_info.exp);
        this->parent->playset_scene->to_level->setNum(this->user_consumer->user_info.level);
        if (this->parent->playset_scene->from_level->text().toInt() < this->user_consumer->user_info.level) {
            this->parent->playset_scene->level_flag->setText("↑");
        } else {
            this->parent->playset_scene->level_flag->setText("-");
        }
        return;
    }
    case client_rpc::sync_status: {
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        QJsonArray arr = rets.toArray();
        this->user_status = UserStatus(static_cast<uint8_t>(arr[0].toInt()));
        this->user_author->user_info = UserFullInfo::from_json_array(arr[1].toArray());
        this->user_consumer->user_info = UserFullInfo::from_json_array(arr[2].toArray());
        return;
    }
    case client_rpc::query_consumers_brief_info: {
        qDebug() << "settle query_consumers_brief_info ...!!!!";
        qDebug() << "settle query_consumers_brief_info ...!!!!";
        qDebug() << "settle query_consumers_brief_info ...!!!!";
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        std::vector<UserBriefInfo> info_list;
        auto arr = rets.toArray();
        for (int i = 0; i < arr.size(); i++) {
            info_list.push_back(UserBriefInfo::from_json_array(arr.at(i).toArray()));
        }
        this->parent->ranking_consumers_scene->set_page_contain(info_list);
        return;
    }
    case client_rpc::code::query_authors_brief_info: {
        qDebug() << "settle query_authors_brief_info ...!!!!";
        qDebug() << "settle query_authors_brief_info ...!!!!";
        qDebug() << "settle query_authors_brief_info ...!!!!";
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        std::vector<UserBriefInfo> info_list;
        auto arr = rets.toArray();
        for (int i = 0; i < arr.size(); i++) {
            info_list.push_back(UserBriefInfo::from_json_array(arr.at(i).toArray()));
        }
        this->parent->ranking_authors_scene->set_page_contain(info_list);
        return;
    }
    case client_rpc::code::query_consumers_by_name: {
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        return;
    }
    case client_rpc::code::query_consumers_by_id: {
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        return;
    }
    case client_rpc::code::query_authors_by_name: {
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        return;
    }
    case client_rpc::code::query_authors_by_id: {
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        return;
    }
    case client_rpc::code::query_users: {
        if (err != nullptr) {
            MessageBox::critical(this->parent, tr("错误"), err);
            return ;
        }
        return;
    }
    default:
        return;
    }
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

//    _last_error = "timeout";
//    bool success = false;
//    QJsonValue ret;
//    int id;
//    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
//        qDebug() << "ret" << bytes_json;
//        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
//    }, 3000);
//    qDebug() << ret;
//    if (success == false) {
//        return false;
//    }
//    this->user_author->user_info = UserFullInfo::from_json_array(ret.toArray());
//    user_status_util::add_author_status(this->user_status);
    return true;
}

bool RemoteClient::author_sign_up(QString account, QString password)
{
    this->handler->write_package(client_rpc::author_sign_up_request(account, password));

//    _last_error = "timeout";
//    bool success = false;
//    QJsonValue ret;
//    int id;
//    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
//        qDebug() << "ret" << bytes_json;
//        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
//    }, 3000);
//    qDebug() << ret;
//    if (success == false) {
//        return false;
//    }
//    this->user_author->user_info = UserFullInfo::from_json_array(ret.toArray());
//    user_status_util::add_author_status(this->user_status);

    return true;
}

bool RemoteClient::consumer_sign_in(QString account, QString password)
{
    this->handler->write_package(client_rpc::consumer_sign_in_request(account, password));

//    _last_error = "timeout";
//    bool success = false;
//    QJsonValue ret;
//    int id;
//    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
//        qDebug() << "ret" << bytes_json;
//        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
//    }, 3000);
//    qDebug() << ret;
//    if (success == false) {
//        return false;
//    }
//    this->user_consumer->user_info = UserFullInfo::from_json_array(ret.toArray());
//    user_status_util::add_consumer_status(this->user_status);

    return true;
}

bool RemoteClient::consumer_sign_up(QString account, QString password)
{
    this->handler->write_package(client_rpc::consumer_sign_up_request(account, password));

//    _last_error = "timeout";
//    bool success = false;
//    QJsonValue ret;
//    int id;
//    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
//        qDebug() << "ret" << bytes_json;
//        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
//    }, 3000);
//    qDebug() << ret;
//    if (success == false) {
//        return false;
//    }
//    this->user_consumer->user_info = UserFullInfo::from_json_array(ret.toArray());
//    user_status_util::add_consumer_status(this->user_status);

    return true;
}


bool RemoteClient::logout()
{
    this->handler->write_package(client_rpc::logout_requset());
//    _last_error = "timeout";
//    bool success = false;
//    QJsonValue ret;
//    int id;
//    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
//        qDebug() << "ret" << bytes_json;
//        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
//    }, 3000);
//    qDebug() << ret;
//    if (success == false) {
//        return false;
//    }
//    this->user_status = UserStatus(static_cast<uint8_t>(ret.toInt()));

    return true;
}

bool RemoteClient::sync_status()
{
    this->handler->write_package(client_rpc::sync_status_request());
//    _last_error = "timeout";
//    bool success = false;
//    QJsonValue ret;
//    int id;
//    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
//        qDebug() << "ret" << bytes_json;
//        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
//    }, 3000);
//    qDebug() << ret;
//    if (success == false) {
//        return false;
//    }
//    QJsonArray arr = ret.toArray();
//    this->user_status = UserStatus(static_cast<uint8_t>(arr[0].toInt()));
//    this->user_author->user_info = UserFullInfo::from_json_array(arr[1].toArray());
//    this->user_consumer->user_info = UserFullInfo::from_json_array(arr[2].toArray());

    return true;
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
    qDebug() << "consu";
    return user_status_util::has_consumer_status(this->user_status);
}

bool RemoteClient::author_logining()
{
    qDebug() << "autho";
    return user_status_util::has_author_status(this->user_status);
}

bool RemoteClient::submit_tango_items(const std::vector<TangoPair> &tango_list)
{
    this->handler->write_package(client_rpc::submit_tango_items_request(tango_list));
//    _last_error = "timeout";
//    bool success = false;
//    QJsonValue ret;
//    int id;
//    this->handler->wait_for_new_package([&](QByteArray bytes_json) mutable {
//        qDebug() << "ret" << bytes_json;
//        success = client_rpc::decode_json_rets_object(bytes_json, ret, id, _last_error);
//    }, 3000);
//    qDebug() << ret;
//    if (success == false) {
//        return false;
//    }
//    this->user_author->user_info = UserFullInfo::from_json_array(ret.toArray());
    return true;
}

AbstractGameAutomation *RemoteClient::start_game_event(const GameConfig *, int n, RetriveMode mode)
{
    this->handler->write_package(client_rpc::start_game_event_request(n, mode));
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
        return nullptr;
    }
    qDebug() << "start game event";
    return new GameAutomationRelayer(this);
}

void RemoteClient::game_start()
{
    this->handler->write_package(client_rpc::signal_start_game_request());
}


void RemoteClient::game_stop()
{
    this->handler->write_package(client_rpc::signal_game_stop_request());
}

void RemoteClient::game_answer(QString tango)
{
    this->handler->write_package(client_rpc::signal_game_answer_request(tango));
}

bool RemoteClient::settle_game_event(const AbstractGameAutomation *)
{
    this->handler->write_package(client_rpc::settle_game_event_request());
    return true;
}

bool RemoteClient::query_authors_brief_info(std::vector<UserBriefInfo>&, int l, int r)
{
    this->handler->write_package(client_rpc::query_authors_brief_info_request(l, r));
    return true;
}

bool RemoteClient::query_consumers_brief_info(std::vector<UserBriefInfo>&, int l, int r)
{
    this->handler->write_package(client_rpc::query_consumers_brief_info_request(l, r));
    return true;
}

bool RemoteClient::query_authors_by_id(UserFullInfo &, int id)
{
    this->handler->write_package(client_rpc::query_authors_by_id_request(id));
    return true;
}

bool RemoteClient::query_consumers_by_id(UserFullInfo &, int id)
{
    this->handler->write_package(client_rpc::query_consumers_by_id_request(id));
    _last_error = "TODO";
    return true;
}

bool RemoteClient::query_consumers_by_name(UserFullInfo &, QString name)
{
    this->handler->write_package(client_rpc::query_consumers_by_name_request(name));
    _last_error = "TODO";
    return true;
}

bool RemoteClient::query_users(int &query_count)
{
    this->handler->write_package(client_rpc::query_users_request());
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
    query_count = ret.toInt();
    return true;
}

bool RemoteClient::query_authors_by_name(UserFullInfo &, QString name)
{
    this->handler->write_package(client_rpc::query_authors_by_name_request(name));
    return true;
}

const QString RemoteClient::last_error()
{
    return _last_error;
}
