
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
#include "GameAutomation.h"
#include "GameConfig.h"


Client::Client(QObject *parent) : QObject(parent)
{
    this->user_author = nullptr;
    this->user_consumer = nullptr;
    this->user_status = UserStatus::None;

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

    qDebug() << "might be open" << this->local_handler.isOpen();
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

bool Client::author_sign_in(QString account, QString password)
{
    if (user_status_util::has_author_status(this->user_status)) {
        qDebug() << "user_doesn't logout";
        user_author->deleteLater();
        user_author = nullptr;
        user_status_util::remove_author_status(this->user_status);
    }
    return this->_author_sign_in(account, password);
}

bool Client::author_sign_up(QString account, QString password)
{
    if (user_status_util::has_author_status(this->user_status)) {
        qDebug() << "author doesn't logout";
        user_author->deleteLater();
        user_author = nullptr;
        user_status_util::remove_author_status(this->user_status);
    }
    return this->_author_sign_up(account, password);
}

bool Client::consumer_sign_in(QString account, QString password)
{
    if (user_status_util::has_consumer_status(this->user_status)) {
        qDebug() << "consumer doesn't logout";
        user_consumer->deleteLater();
        user_consumer = nullptr;
        user_status_util::remove_consumer_status(this->user_status);
    }
    return this->_consumer_sign_in(account, password);
}

bool Client::consumer_sign_up(QString account, QString password)
{
    if (user_status_util::has_consumer_status(this->user_status)) {
        qDebug() << "consumer doesn't logout";
        user_consumer->deleteLater();
        user_consumer = nullptr;
        user_status_util::remove_consumer_status(this->user_status);
    }
    return this->_consumer_sign_up(account, password);
}

bool Client::logout()
{
    return this->logout_local();
}

int Client::consumer_exp()
{
    if (user_status_util::has_consumer_status(this->user_status)) {
        return this->user_consumer->exp;
    }
    qDebug() << "consumer exp=0";
    return -1;
}

int Client::consumer_level()
{
    if (user_status_util::has_consumer_status(this->user_status)) {
        return this->user_consumer->level;
    }
    return -1;
}

bool Client::submit_tango_items(const std::vector<TangoPair> &tango_list)
{
    if (!user_status_util::has_author_status(this->user_status)) {
        this->_last_error = "author doesn't sign in";
        return false;
    }
    return this->_submit_tango_items(tango_list);
}

GameAutomation *Client::start_game_event(const GameConfig *game_config, int n, RetriveMode mode)
{
    if (!user_status_util::has_consumer_status(this->user_status)) {
        this->_last_error = "consumer doesn't sign in";
        return nullptr;
    }
    return start_game_event_local(game_config, n, mode);
}

bool Client::settle_game_event(const GameAutomation *automate)
{
    if (!user_status_util::has_consumer_status(this->user_status)) {
        this->_last_error = "consumer doesn't sign in";
        return false;
    }
    return settle_game_event_local(automate);
}

bool Client::query_authors_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    if (l < 0 || r < 0) {
        this->_last_error = "range must not be negative";
        return false;
    }
    if (l > r) {
        this->_last_error = "l must be less than or equal to r";
        return false;
    }
    return query_authors_brief_info_local(info_list, l, r);
}

bool Client::query_consumers_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    if (l < 0 || r < 0) {
        this->_last_error = "range must not be negative";
        return false;
    }
    if (l > r) {
        this->_last_error = "l must be less than or equal to r";
        return false;
    }
    return query_consumers_brief_info_local(info_list, l, r);
}

bool Client::query_authors_by_id(UserFullInfo &query_container, int id)
{
    return this->query_authors_by_id_local(query_container, id);
}

bool Client::query_consumers_by_id(UserFullInfo &query_container, int id)
{
    return this->query_consumers_by_id_local(query_container, id);
}

bool Client::query_consumers_by_name(UserFullInfo &query_container, QString name)
{
    return this->query_consumers_by_name_local(query_container, name);
}

bool Client::query_authors_by_name(UserFullInfo &query_container, QString name)
{
    return this->query_authors_by_name_local(query_container, name);
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

// 假定author已下线
bool Client::author_sign_in_local(QString account, QString password)
{
    user_author = new class Author(this->local_handler);
    if (user_author->sign_in_local(account, password)) {
        user_status_util::add_author_status(this->user_status);

        return true;
    }

    _last_error = user_author->last_error();
    user_author->deleteLater();
    user_author = nullptr;

    return false;
}

// 假定author已下线
bool Client::author_sign_up_local(QString account, QString password)
{
    user_author = new class Author(this->local_handler);
    if (user_author->sign_up_local(account, password)) {
        user_status_util::add_author_status(this->user_status);

        return true;
    }

    _last_error = user_author->last_error();
    user_author->deleteLater();
    user_author = nullptr;

    return false;
}

// 假定consumer已下线
bool Client::consumer_sign_in_local(QString account, QString password)
{
    user_consumer = new class Consumer(this->local_handler);
    if (user_consumer->sign_in_local(account, password)) {
        user_status_util::add_consumer_status(this->user_status);

        return true;
    }

    _last_error = user_consumer->last_error();
    user_consumer->deleteLater();
    user_consumer = nullptr;

    return false;
}


// 假定consumer已下线
bool Client::consumer_sign_up_local(QString account, QString password)
{
    user_consumer = new class Consumer(this->local_handler);
    if (user_consumer->sign_up_local(account, password)) {
        user_status_util::add_consumer_status(this->user_status);

        return true;
    }

    _last_error = user_consumer->last_error();
    user_consumer->deleteLater();
    user_consumer = nullptr;

    return false;
}

bool Client::logout_local()
{
    qDebug() << "logouting";
    if (user_status_util::has_author_status(this->user_status)) {
        qDebug() << "logout author";
        if (user_author->login_out_local()) {
            user_author->deleteLater();
            user_author = nullptr;
            user_status_util::remove_author_status(this->user_status);
        } else {
            _last_error = user_author->last_error();
            qDebug() << "error occured" << _last_error;
            return false;
        }
    }
    if (user_status_util::has_consumer_status(this->user_status)) {
        qDebug() << "logout consumer" << user_consumer->tango_count;
        if (user_consumer->login_out_local()) {
            user_consumer->deleteLater();
            user_consumer = nullptr;
            user_status_util::remove_consumer_status(this->user_status);
        } else {
            _last_error = user_consumer->last_error();
            qDebug() << "error occured" << _last_error;
            return false;
        }
    }

    return true;
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

bool Client::consumer_sign_in_remote(QString account, QString password)
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

bool Client::consumer_sign_up_remote(QString account, QString password)
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

// 假定author已上线
bool Client::submit_tango_items_local(const std::vector<TangoPair> &tango_list)
{
    static const char *insert_command =
        "insert into `tangos` ( `key`, `value`, `last_submit`) "
        "               values (:key,  :value,  :ls)";

    qDebug() << "tango list" << tango_list;
    QSqlQuery query(this->local_handler);
    if (!query.exec("start transaction")) {
        qDebug() << "start transaction error" << query.lastError().text();
        _last_error = query.lastError().text();
        return false;
    }

    query.prepare(insert_command);
    query.bindValue(":ls", this->user_author->name);

    for (unsigned int i = 0; i < tango_list.size(); i++) {
        query.bindValue(":key", tango_list[i].first);
        query.bindValue(":value", tango_list[i].second);

        if (!query.exec()) {
             qDebug() << "exec error" << query.lastError().text();
             _last_error = query.lastError().text();
             if (!query.exec("rollback")) {
                 qDebug() << "rollback error" << query.lastError().text();
                 _last_error = query.lastError().text();
                 return false;
             }
             return false;
        }
    }

    if (!query.exec("commit")) {
        qDebug() << "commit error" << query.lastError().text();
        _last_error = query.lastError().text();
        return false;
    }

    this->settle_creation_event_local(tango_list);
    return true;
}

bool Client::submit_tango_items_remote(const std::vector<TangoPair> &tango_list)
{
    qDebug() << "tango list" << tango_list;
    _last_error = "TODO";

    return false;
}


// 假定author已上线
bool Client::settle_creation_event_local(const std::vector<TangoPair> &tango_list)
{
    this->user_author->misson_count++;
    for (unsigned int i = 0; i < tango_list.size(); i++) {
        this->user_author->exp += tango_list[i].first.length() + tango_list[i].second.length();
    }
    this->user_author->tango_count += tango_list.size();
    while (this->user_author->level * 10 + 10 <= this->user_author->exp) {
        this->user_author->exp -= this->user_author->level * 10 + 10;
        this->user_author->level++;
    }
    return true;
}


bool Client::retrive_kth_tango_item_local(TangoPair &tp, int k) {
    static const char *search_command = "select * from `tangos` order by length(key) limit :kth - 1, 1";

    QSqlQuery query(this->local_handler);
    query.prepare(search_command);
    query.bindValue(":kth", k);

    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }

    if (query.first()) {
        tp = TangoPair(query.value(1).toString(), query.value(2).toString());

        return true;
    }
    _last_error = "not found";

    return false;
}

int Client::retrive_since_kth_tango_item_local(std::vector<TangoPair> &tango_list, unsigned int k, int n)
{
    static const char *search_command = "select * from `tangos` order by length(`key`) limit :kth, :ntimes";

    QSqlQuery query(this->local_handler);
    query.prepare(search_command);
    query.bindValue(":kth", k);
    query.bindValue(":ntimes", n);
    qDebug() << "k, n" << k << " " << n;
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return 0;
    }

    int ret = 0;
    while (query.next()) {
        tango_list.push_back(TangoPair(query.value(1).toString(), query.value(2).toString()));
        qDebug() << "fetched " << TangoPair(query.value(1).toString(), query.value(2).toString());
        ret++;
    }

    return ret;
}


bool Client::retrive_tango_items_local(std::vector<TangoPair> &tango_list, int &n, RetriveMode mode)
{
    static std::mt19937 mtrand(static_cast<unsigned int>(time(nullptr)));
    static const char *query_count = "select count(*) from `tangos`";
    QSqlQuery query(this->local_handler);
    if (!query.exec(query_count)) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;
        return false;
    }
    if (!query.first()) {
        _last_error = "first fetch error";
        qDebug() << "error occured: " << _last_error;
        return false;
    }

    int tot_length = query.value(0).toInt();
    qDebug() << "tot_length " << tot_length;

    if (tot_length == 0) {
        _last_error = "empty pool in the tango database";
        return false;
    }

    unsigned int to_fetch;
    switch (mode) {
    case RetriveMode::Easy:
        tot_length = static_cast<int>(0.3 * tot_length);
        if (tot_length < n) {
            n = std::max(tot_length, 1);
        }
        to_fetch = mtrand() % static_cast<unsigned int>(tot_length - n + 1);
        _last_error = "";
        if (!this->retrive_since_kth_tango_item_local(tango_list, to_fetch, tot_length)) {
            if (_last_error != "") {
                _last_error = "fetch error";
            }
            return false;
        }
        return true;

    case RetriveMode::Normal:
        tot_length = static_cast<int>(0.6 * tot_length);
        if (tot_length < n) {
            n = std::max(tot_length, 1);
        }
        to_fetch = mtrand() % static_cast<unsigned int>(tot_length - n + 1);
        _last_error = "";
        if (!this->retrive_since_kth_tango_item_local(tango_list, to_fetch, tot_length)) {
            if (_last_error != "") {
                _last_error = "fetch error";
            }
            return false;
        }
        return true;

    case RetriveMode::Hard:
        if (tot_length < n) {
            n = std::max(tot_length, 1);
        }
        to_fetch = mtrand() % static_cast<unsigned int>(tot_length - n + 1);
        _last_error = "";
        if (!this->retrive_since_kth_tango_item_local(tango_list, to_fetch, tot_length)) {
            if (_last_error != "") {
                _last_error = "fetch error";
            }
            return false;
        }
        return true;
    }

    return true;
}

// 假定consumer已上线
GameAutomation *Client::start_game_event_local(const GameConfig *game_config, int n, RetriveMode mode=RetriveMode::Hard)
{
    std::vector<TangoPair> tango_list;
    tango_list.reserve(static_cast<unsigned int>(n));
    qDebug() << "want " << n;
    int temp_n = n;
    if (!this->retrive_tango_items_local(tango_list, temp_n, mode)) {
        return nullptr;
    }

    auto automate = new GameAutomation(game_config);
    if (!automate->prepare_start(tango_list, static_cast<unsigned int>(temp_n))) {
        _last_error = automate->last_error();
        automate->deleteLater();
        return nullptr;
    }
    return automate;
}

// 假定consumer已上线
bool Client::settle_game_event_local(const GameAutomation *automate)
{
    qDebug() << this->user_consumer << this->user_consumer->misson_count << this->user_consumer->exp << this->user_consumer->tango_count << this->user_consumer->level;
    if (static_cast<unsigned int>(automate->success_count) < automate->tango_pool->size()) {
        this->user_consumer->misson_count++;
    }
    this->user_consumer->exp += automate->exp;
    this->user_consumer->tango_count += automate->success_count;
    while (this->user_consumer->level * 10 + 10 <= this->user_consumer->exp) {
        this->user_consumer->exp -= this->user_consumer->level * 10 + 10;
        this->user_consumer->level++;
    }
    return true;
}

// 假定 l <= r
bool Client::query_authors_brief_info_local(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    static const char *query_command =
        "select `id`, `name`, `level` from `authors`"
        "     order by `level` desc, `exp` desc limit :kth, :ntimes";

    QSqlQuery query(this->local_handler);
    query.prepare(query_command);
    query.bindValue(":kth", l);
    query.bindValue(":ntimes", r-l+1);
    qDebug() << "k, n" << l << " " << r-l+1;
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }

    while (query.next()) {
        info_list.emplace_back(UserBriefInfo(query.value(0).toInt(), query.value(1).toString(), query.value(2).toInt()));
        qDebug() << "fetched " << query.value(0).toInt() << query.value(1).toString() << query.value(2).toInt();
    }

    return true;
}

bool Client::query_authors_by_id_local(UserFullInfo &query_container, int id)
{
    static const char *query_command = "select * from `authors` where `id` = :id";

    QSqlQuery query(this->local_handler);
    query.prepare(query_command);
    query.bindValue(":id", id);
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }
    if (!query.first()) {
        _last_error = query.lastError().text();
        qDebug() << "fetch first element error occured: " << _last_error;

        return false;
    }

    query_container.user_id = query.value(0).toInt();
    query_container.name = query.value(1).toString();
    query_container.tango_count = query.value(5).toInt();
    query_container.misson_count = query.value(6).toInt();
    query_container.exp = query.value(3).toInt();
    query_container.level = query.value(4).toInt();
    query_container.motto = query.value(7).toString();

    return true;
}

bool Client::query_consumers_by_id_local(UserFullInfo &query_container, int id)
{
    static const char *query_command = "select * from `consumers` where `id` = :id";

    QSqlQuery query(this->local_handler);
    query.prepare(query_command);
    query.bindValue(":id", id);
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }
    if (!query.first()) {
        _last_error = query.lastError().text();
        qDebug() << "fetch first element error occured: " << _last_error;

        return false;
    }

    query_container.user_id = query.value(0).toInt();
    query_container.name = query.value(1).toString();
    query_container.tango_count = query.value(5).toInt();
    query_container.misson_count = query.value(6).toInt();
    query_container.exp = query.value(3).toInt();
    query_container.level = query.value(4).toInt();
    query_container.motto = query.value(7).toString();

    return true;
}

bool Client::query_authors_by_name_local(UserFullInfo &query_container, QString name)
{
    static const char *query_command = "select * from `authors` where `name` = :name";

    QSqlQuery query(this->local_handler);
    query.prepare(query_command);
    query.bindValue(":name", name);
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }
    if (!query.first()) {
        _last_error = query.lastError().text();
        qDebug() << "fetch first element error occured: " << _last_error;

        return false;
    }

    query_container.user_id = query.value(0).toInt();
    query_container.name = query.value(1).toString();
    query_container.tango_count = query.value(5).toInt();
    query_container.misson_count = query.value(6).toInt();
    query_container.exp = query.value(3).toInt();
    query_container.level = query.value(4).toInt();
    query_container.motto = query.value(7).toString();

    return true;
}

bool Client::query_consumers_by_name_local(UserFullInfo &query_container, QString name)
{
    static const char *query_command = "select * from `consumers` where `name` = :name";

    QSqlQuery query(this->local_handler);
    query.prepare(query_command);
    query.bindValue(":name", name);
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }
    if (!query.first()) {
        _last_error = query.lastError().text();
        qDebug() << "fetch first element error occured: " << _last_error;

        return false;
    }

    query_container.user_id = query.value(0).toInt();
    query_container.name = query.value(1).toString();
    query_container.tango_count = query.value(5).toInt();
    query_container.misson_count = query.value(6).toInt();
    query_container.exp = query.value(3).toInt();
    query_container.level = query.value(4).toInt();
    query_container.motto = query.value(7).toString();

    return true;
}

// 假定 l <= r
bool Client::query_consumers_brief_info_local(std::vector<UserBriefInfo> &info_list, int l, int r)
{
    static const char *query_command =
        "select `id`, `name`, `level` from `consumers`"
        "     order by `level` desc, `exp` desc limit :kth, :ntimes";

    QSqlQuery query(this->local_handler);
    query.prepare(query_command);
    query.bindValue(":kth", l);
    query.bindValue(":ntimes", r-l+1);
    qDebug() << "k, n" << l << " " << r-l+1;
    if (!query.exec()) {
        _last_error = query.lastError().text();
        qDebug() << "error occured: " << _last_error;

        return false;
    }

    while (query.next()) {
        info_list.emplace_back(UserBriefInfo(query.value(0).toInt(), query.value(1).toString(), query.value(2).toInt()));
        qDebug() << "fetched " << query.value(0).toInt() << query.value(1).toString() << query.value(2).toInt();
    }

    return true;
}


std::function<void()> Client::switch_remote_mode_slottor()
{
    return [this]() mutable {

        this->_author_sign_in = [this](QString account, QString password) mutable {
            return this->author_sign_in_remote(account, password);
        };
        this->_author_sign_up = [this](QString account, QString password) mutable {
            return this->author_sign_up_remote(account, password);
        };
        this->_consumer_sign_in = [this](QString account, QString password) mutable {
            return this->consumer_sign_in_remote(account, password);
        };
        this->_consumer_sign_up = [this](QString account, QString password) mutable {
            return this->consumer_sign_up_remote(account, password);
        };
        this->_submit_tango_items = [this](const std::vector<TangoPair> &tango_list) mutable {
            return this->submit_tango_items_remote(tango_list);
        };
        this->_is_connected = [this]() mutable -> bool {
            return this->is_remote_server_connected();
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

        this->_author_sign_in = [this](QString account, QString password) mutable {
            return this->author_sign_in_local(account, password);
        };
        this->_author_sign_up = [this](QString account, QString password) mutable {
            return this->author_sign_up_local(account, password);
        };
        this->_consumer_sign_in = [this](QString account, QString password) mutable {
            return this->consumer_sign_in_local(account, password);
        };
        this->_consumer_sign_up = [this](QString account, QString password) mutable {
            return this->consumer_sign_up_local(account, password);
        };
        this->_submit_tango_items = [this](const std::vector<TangoPair> &tango_list) mutable {
            return this->submit_tango_items_local(tango_list);
        };
        this->_is_connected = [this]() mutable -> bool {
            return this->is_local_handler_connected();
        };
    };
}

const QString Client::last_error()
{
    return this->_last_error;
}

bool Client::is_connected()
{
    return this->_is_connected();
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

bool Client::create_consumer_table()
{
    static const char *create_command =
        "create table if not exists `consumers` ("
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
        " ) engine=InnoDB default charset=utf8 comment='consumers that enjoy the happiness the tango world'";

    QSqlQuery query(this->local_handler);

    query.exec("set names 'utf8'");
    if (!query.exec(create_command)) {
        qDebug() << "create consumers table failed" << query.lastError().text();
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
        "    `last_submit` varchar(45) DEFAULT NULL,"
        "    `last_submit_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,"
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
    if (!this->create_consumer_table()) {
        return false;
    }
    if (!this->create_tangos_table()) {
        return false;
    }

    return true;
}
