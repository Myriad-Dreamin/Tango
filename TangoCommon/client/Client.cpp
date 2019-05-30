
#include "Client.h"

#include <set>
#include <ctime>
#include <random>

#include <QTcpSocket>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>

#include "../players/Author.h"
#include "../players/Consumer.h"

#include "../types/TangoPair.h"
#include "../types/RetriveMode.h"
#include "../types/UserBriefInfo.h"
#include "../types/UserFullInfo.h"

#include "../component/MessageBox.h"
#include "../component/ConfigSet.h"

#include "../automator/GameAutomation.h"
#include "../automator/GameConfig.h"

#include "AbstractClient.h"
#include "LocalClient.h"
#include "RemoteClient.h"


Client::Client(QObject *parent) : QObject(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);
    this->handler = nullptr;
    this->local_handler = new LocalClient(this);
    this->remote_handler = new RemoteClient(parent);
}

Client::~Client()
{
    qDebug() << "release client";
    if (this->remote_handler != nullptr) {
        this->remote_handler->stop_connection();
    }
    if (this->local_handler != nullptr) {
         this->local_handler->stop_connection();
    }
}


bool Client::setup_remote_connection(QHostAddress host_address, quint16 server_port)
{
    qDebug() << "trying local connection";
    if (this->remote_handler == nullptr) {
        _last_error = "remote_handler is not inited";
        return false;
    }
    if (!this->remote_handler->setup_connection(host_address, server_port)) {
        this->_last_error = this->remote_handler->last_error();
        this->handler = nullptr;
        return false;
    }
    this->handler = remote_handler;
    return true;
}


bool Client::setup_local_connection()
{
    qDebug() << "trying local connection";
    if (this->local_handler == nullptr) {
        _last_error = "local_handler is not inited";
        return false;
    }
    this->local_handler->reset_database_config(
        this->parent->qconfig->at("mysql/host").toString(),
        this->parent->qconfig->at("mysql/basename").toString()
    );

    this->local_handler->reset_database_user(
        this->parent->qconfig->at("mysql/user").toString(),
        this->parent->qconfig->at("mysql/password").toString()
    );

    if (!this->local_handler->setup_connection()) {
        this->_last_error = this->local_handler->last_error();
        qDebug() << "..." << this->local_handler->last_error();
        this->handler = nullptr;
        return false;
    }
    this->handler = local_handler;
    return true;
}

bool Client::stop_local_connection()
{
    if (this->local_handler == nullptr) {
        _last_error = "local_handler is not inited";
        return false;
    }
    if (!this->local_handler->stop_connection()) {
        this->_last_error = this->local_handler->last_error();
        return false;
    }
    this->local_handler = nullptr;
    return true;
}

bool Client::is_remote_server_connected()
{
    if (this->remote_handler == nullptr) {
        _last_error = "remote_handler is not inited";
        return false;
    }
    return this->remote_handler->is_connected();
}

bool Client::is_local_handler_connected()
{
    if (this->local_handler == nullptr) {
        _last_error = "local_handler is not inited";
        return false;
    }
    return this->local_handler->is_connected();
}

bool Client::author_sign_in(QString account, QString password)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->author_sign_in(account, password)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::author_sign_up(QString account, QString password)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->author_sign_up(account, password)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::consumer_sign_in(QString account, QString password)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->consumer_sign_in(account, password)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::consumer_sign_up(QString account, QString password)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->consumer_sign_up(account, password)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::logout()
{
    qDebug() << "logout";
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    qDebug() << "logout" << this->handler;
    if (this->handler->logout()) {
        qDebug() << "logout";
        return true;
    }
    qDebug() << "logout";
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::sync_status()
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->sync_status()) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

int Client::consumer_exp()
{
    return this->handler->consumer_exp();
}

int Client::consumer_level()
{
    return this->handler->consumer_level();
}

bool Client::consumer_logining()
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->consumer_logining()) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::author_logining()
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->author_logining()) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::submit_tango_items(const std::vector<TangoPair> &tango_list)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->submit_tango_items(tango_list)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

AbstractGameAutomation *Client::start_game_event(const GameConfig *game_config, int n, RetriveMode mode)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return nullptr;
    }
    auto qwq = this->handler->start_game_event(game_config, n, mode);
    if (qwq) {
        return qwq;
    }
    this->_last_error = this->handler->last_error();
    return qwq;
}

bool Client::settle_game_event(const AbstractGameAutomation *automate)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->settle_game_event(automate)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::query_authors_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->query_authors_brief_info(info_list, l, r)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::query_consumers_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->query_consumers_brief_info(info_list, l, r)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::query_authors_by_id(UserFullInfo &query_container, int id)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->query_authors_by_id(query_container, id)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::query_consumers_by_id(UserFullInfo &query_container, int id)
{
    if (this->handler->query_consumers_by_id(query_container, id)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::query_consumers_by_name(UserFullInfo &query_container, QString name)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->query_consumers_by_name(query_container, name)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::query_users(int &query_count)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->query_users(query_count)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

bool Client::query_online_users()
{
    if (this->remote_handler == nullptr) {
        _last_error = "remote handler is not inited";
        return false;
    }
    this->remote_handler->query_online_users();
    return true;
}

bool Client::query_authors_by_name(UserFullInfo &query_container, QString name)
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    if (this->handler->query_authors_by_name(query_container, name)) {
        return true;
    }
    this->_last_error = this->handler->last_error();
    return false;
}

const QString Client::last_error()
{
    return this->_last_error;
}

bool Client::is_connected()
{
    if (this->handler == nullptr) {
        _last_error = "handler is not inited";
        return false;
    }
    return this->handler->is_connected();
}

bool Client::init_default_tangos()
{
    if (this->local_handler != nullptr) {
        return this->local_handler->init_default_tangos();
    }
    _last_error = "local handler is not inited";
    return false;
}



bool Client::create_tables()
{
    if (this->local_handler != nullptr) {
        return this->local_handler->create_tables();
    }
    _last_error = "local handler is not inited";
    return false;
}
