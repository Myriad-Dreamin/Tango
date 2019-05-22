

#include "TangoThread.h"
#include "../../TangoCommon/network/SocketX.h"

#include <QJsonDocument>
#include "../../TangoCommon/client/json_rpc.h"
#include "../../TangoCommon/client/LocalClient.h"
#include "../../TangoCommon/client/GameConfig.h"

TangoThread::TangoThread(long long sock_desc, QObject *parent) :
    QThread(parent),
    client_id(sock_desc)
{
    this->client = new LocalClient(this);
    m_socket = new SocketX(client_id);
    automate_started = false;
    game_event_enable = false;
    this->game_config = nullptr;
    this->automate = nullptr;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(this, &QThread::finished, this->client, &LocalClient::logout);
}


TangoThread::TangoThread(long long sock_desc, QSqlDatabase &tango_sql, QObject *parent) :
    QThread(parent),
    client_id(sock_desc)
{
    this->client = new LocalClient(tango_sql, this);
    m_socket = new SocketX(client_id);
    automate_started = false;
    game_event_enable = false;
    this->game_config = nullptr;
    this->automate = nullptr;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(this, &QThread::finished, this->client, &LocalClient::logout);
}

TangoThread::~TangoThread()
{
    if (this->game_config != nullptr) {
        delete this->game_config;
    }
    if (automate != nullptr) {
        automate->deleteLater();
    }

    this->client->deleteLater();
    m_socket->close();
}

void TangoThread::run(void)
{

    if (!m_socket->setSocketDescriptor(client_id)) {
        return ;
    }

    connect(m_socket, &SocketX::package_ready, [this](const QString &ip, const quint16 &port, const QByteArray &data) mutable {
        qDebug() << "package received" << ip << port << data;

        QString err;
        QJsonArray params;
        int id;
        if (!client_rpc::decode_json_params_object(data, params, id, err)) {
            qDebug() << err;
            return;
        }
        switch(id) {
        case client_rpc::code::author_sign_in: {
            if (params.size() != 2) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::author_sign_in));
                return;
            }
            QString account;
            QString password;
            account = params[0].toString("");
            password = params[1].toString("");

            if (account == "" || password == "") {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::author_sign_in));
                return;
            }

            m_socket->write_package(this->author_sign_in(account, password));
            return;
        }
        case client_rpc::code::author_sign_up: {
            if (params.size() != 2) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::author_sign_up));
                return;
            }
            QString account;
            QString password;
            account = params[0].toString("");
            password = params[1].toString("");

            if (account == "" || password == "") {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::author_sign_up));
                return;
            }

            m_socket->write_package(this->author_sign_up(account, password));
            return;
        }
        case client_rpc::code::consumer_sign_in: {
            if (params.size() != 2) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::consumer_sign_in));
                return;
            }
            QString account;
            QString password;
            account = params[0].toString("");
            password = params[1].toString("");

            if (account == "" || password == "") {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::consumer_sign_in));
                return;
            }

            m_socket->write_package(this->consumer_sign_in(account, password));
            return;
        }
        case client_rpc::code::consumer_sign_up: {
            if (params.size() != 2) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::consumer_sign_up));
                return;
            }
            QString account;
            QString password;
            account = params[0].toString("");
            password = params[1].toString("");

            if (account == "" || password == "") {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::consumer_sign_up));
                return;
            }

            m_socket->write_package(this->consumer_sign_up(account, password));
            return;
        }
        case client_rpc::code::logout: {
            if (params.size() != 0) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::logout));
                return;
            }

            m_socket->write_package(this->logout());
            return;
        }
        case client_rpc::code::sync_status: {
            qDebug() << "sync..." << params;
            if (params.size() != 0) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::sync_status));
                return;
            }

            m_socket->write_package(this->sync_status());
            qDebug() << "sync_status ready";
            return;
        }
        case client_rpc::code::submit_tango_items: {

            for (int i = 0; i < params.size(); i++) {
                if (params[i].toArray().size() != 2) {
                    m_socket->write_package(client_rpc::err_invalid_params(client_rpc::code::submit_tango_items));
                    return;
                }
            }
            std::vector<TangoPair> tango_list;
            tango_list.reserve(static_cast<size_t>(params.size()));
            for (int i = 0; i < params.size(); i++) {
                tango_list.push_back(TangoPair::from_json_array(params[i].toArray()));
            }

            m_socket->write_package(this->submit_tango_items(tango_list));
            return;
        }
        case client_rpc::signal_game_answer: {
            if (!game_event_enable) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::signal_game_answer));
                return ;
            }
            if (params.size() != 1) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::signal_game_answer));
                return;
            }
            QString user_ret = params[0].toString();

            qDebug() << user_ret << current_key;

            if (user_ret == current_key) {
                automate->answer_tango(current_key);
            }
            return;
        }
        case client_rpc::signal_game_stop: {
            if (!game_event_enable) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::signal_game_stop));
                return ;
            }
            this->automate->stop();
            return;
        }
        case client_rpc::signal_start_game: {
            if (!game_event_enable) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::signal_start_game));
                return ;
            }
            if (automate_started) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::signal_start_game));
                return;
            }
            automate_started = true;
            automate->start();
            return;
        }
        case client_rpc::start_game_event: {
            qDebug() << "start game event";
            if (automate_started) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::start_game_event));
                return;
            }
            if (params.size() != 2) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::start_game_event));
                return;
            }

            int n = params[0].toInt(2133333333), v = params[1].toInt(2133333333);
            if (n == 2133333333 || v == 2133333333) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::start_game_event));
                return;
            }
            m_socket->write_package(this->start_game_event(n, RetriveMode(v)));
            return;
        }
        case client_rpc::settle_game_event: {
            if (!game_event_enable) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::settle_game_event));
                return;
            }
            this->game_event_enable = false;
            m_socket->write_package(this->settle_game_event());
            qDebug() << "writed";
            return;
        }
        case client_rpc::query_authors_brief_info: {
            if (params.size() != 2) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::query_authors_brief_info));
                return;
            }
            int l = params[0].toInt(2133333333), r = params[1].toInt(2133333333);
            if (l == 2133333333 || r == 2133333333) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::query_authors_brief_info));
                return;
            }
            m_socket->write_package(this->query_authors_brief_info(l, r));
            qDebug() << "writed";
            return;
        }
        case client_rpc::query_consumers_brief_info: {
            if (params.size() != 2) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::query_consumers_brief_info));
                return;
            }
            int l = params[0].toInt(2133333333), r = params[1].toInt(2133333333);
            if (l == 2133333333 || r == 2133333333) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::query_authors_brief_info));
                return;
            }
            m_socket->write_package(this->query_consumers_brief_info(l, r));
            qDebug() << "writed";
            return;
        }
        case client_rpc::query_consumers_by_name: {
            if (params.size() != 2) {
                m_socket->write_package(client_rpc::err_invalid_params(client_rpc::query_consumers_by_name));
                return;
            }
            m_socket->write_package(client_rpc::query_)
        }
        default: {
            m_socket->write_package(client_rpc::err_method_not_found());
            return;
        }
        }
    });


    this->make_on_disconnected_client();
    this->exec();
}

void TangoThread::make_on_disconnected_client()
{
    connect(m_socket, &SocketX::disconnected, [this]() {
        qDebug() << "disconnected Client Server" << this->client_id;
        emit disconnected_from_client(this->client_id);
        m_socket->disconnectFromHost();
    });
}

QByteArray TangoThread::author_sign_in(QString account, QString password)
{
    if (!this->client->author_sign_in(account, password)) {
        return client_rpc::err_exec_error(client_rpc::code::author_sign_in, this->client->last_error());
    }
    qDebug() << "good" << this->client->author_logining();
    return client_rpc::author_sign_in_returns(this->client->author_info());
}


QByteArray TangoThread::author_sign_up(QString account, QString password)
{
    if (!this->client->author_sign_up(account, password)) {
        return client_rpc::err_exec_error(client_rpc::code::author_sign_up, this->client->last_error());
    }
    qDebug() << "good" << this->client->author_logining();
    return client_rpc::author_sign_up_returns(this->client->author_info());
}


QByteArray TangoThread::consumer_sign_in(QString account, QString password)
{
    if (!this->client->consumer_sign_in(account, password)) {
        return client_rpc::err_exec_error(client_rpc::code::consumer_sign_in, this->client->last_error());
    }
    qDebug() << "good" << this->client->consumer_logining();
    return client_rpc::consumer_sign_in_returns(this->client->consumer_info());
}


QByteArray TangoThread::consumer_sign_up(QString account, QString password)
{
    if (!this->client->consumer_sign_up(account, password)) {
        return client_rpc::err_exec_error(client_rpc::code::consumer_sign_up, this->client->last_error());
    }
    qDebug() << "good" << this->client->consumer_logining();
    return client_rpc::consumer_sign_up_returns(this->client->consumer_info());
}

QByteArray TangoThread::logout()
{
    if (!this->client->logout()) {
        return client_rpc::err_exec_error(client_rpc::code::logout, this->client->last_error());
    }
    qDebug() << "good" << this->client->consumer_logining();
    UserStatus ret = UserStatus::None;
    if (this->client->author_logining()) {
        user_status_util::add_author_status(ret);
    }
    if (this->client->consumer_logining()) {
        user_status_util::add_consumer_status(ret);
    }
    return client_rpc::logout_returns(ret);
}


QByteArray TangoThread::sync_status()
{
    if (!this->client->sync_status()) {
        qDebug() << "sync err" << this->client->last_error();
        return client_rpc::err_exec_error(client_rpc::code::sync_status, this->client->last_error());
    }
    qDebug() << "good" << this->client->consumer_logining();
    UserStatus ret = UserStatus::None;
    if (this->client->author_logining()) {
        user_status_util::add_author_status(ret);
    }
    if (this->client->consumer_logining()) {
        user_status_util::add_consumer_status(ret);
    }
    return client_rpc::sync_status_returns(ret, this->client->author_info(), this->client->consumer_info());
}

QByteArray TangoThread::submit_tango_items(std::vector<TangoPair> &tango_list)
{
    if (!this->client->submit_tango_items(tango_list)) {
        return client_rpc::err_exec_error(client_rpc::code::submit_tango_items, this->client->last_error());
    }
    qDebug() << "good";
    return client_rpc::submit_tango_items_returns(this->client->author_info());
}



QByteArray TangoThread::start_game_event(int n, RetriveMode mode)
{
    if (game_config != nullptr) {
        delete game_config;
    }

    switch (mode) {
    case RetriveMode::DefaultMode:{
        game_config = new GameConfig(GameConfigMode::DEFAULT_CONFIG);
        break;
    }
    case RetriveMode::EasyMode:{
        game_config = new GameConfig(GameConfigMode::HARD_EASYMODE_CONFIG);
        break;
    }
    case RetriveMode::NormalMode:{
        game_config = new GameConfig(GameConfigMode::HARD_NORMALMODE_CONFIG);
        break;
    }
    case RetriveMode::HardMode:{
        game_config = new GameConfig(GameConfigMode::HARD_HARDMODE_CONFIG);
        break;
    }
    default:
        return client_rpc::err_invalid_params(client_rpc::start_game_event);
    }
    if (automate != nullptr) {
        automate->deleteLater();
    }
    automate = this->client->start_game_event(game_config, n, RetriveRange::get_mode(mode));

    if (automate == nullptr) {
        return client_rpc::err_exec_error(client_rpc::start_game_event, this->client->last_error());
    }

    connect(automate, &AbstractGameAutomation::start_game, [this]() mutable {
        qDebug() << "start game";
        this->m_socket->write_package(client_rpc::signal_start_game_request());
    });
    connect(automate, &AbstractGameAutomation::new_tango, [this](TangoPair tango, int fade_time) mutable {
        qDebug() << "retreving" << tango;
        current_key = tango.first;
        this->m_socket->write_package(client_rpc::signal_new_tango_request(tango, fade_time));
    });
    connect(automate, &AbstractGameAutomation::tango_faded, [this](int answer_time) mutable {
        qDebug() << "tango faded event";
        this->m_socket->write_package(client_rpc::signal_tango_faded_request(answer_time));
    });
    connect(automate, &AbstractGameAutomation::answer_failed, []() mutable {
        qDebug() << "answer failed";
    });

    connect(automate, &AbstractGameAutomation::success, [this]() mutable {
        qDebug() << "success";
        this->m_socket->write_package(client_rpc::signal_game_success_request());
        automate_started = false;
    });
    connect(automate, &AbstractGameAutomation::failed, [this]() mutable {
        qDebug() << "failed";
        this->m_socket->write_package(client_rpc::signal_game_failed_request());
        this->automate_started = false;
    });

    game_event_enable = true;
    automate_started = false;
    return client_rpc::success(client_rpc::start_game_event);
}
// QByteArray start_game_event_returns(const AbstractGameAutomation *automate);

QByteArray TangoThread::settle_game_event()
{
    if (!this->client->settle_game_event(automate)) {
        return client_rpc::err_exec_error(client_rpc::settle_game_event, this->client->last_error());
    }
    qDebug() << "sending syncing info" <<  client_rpc::settle_game_event_returns(this->client->consumer_info());
    return client_rpc::settle_game_event_returns(this->client->consumer_info());
}
// QByteArray settle_game_event_returns();
QByteArray TangoThread::query_authors_brief_info(int l, int r)
{
    std::vector<UserBriefInfo> info_list;
    if (!this->client->query_authors_brief_info(info_list, l, r)) {
        return client_rpc::err_exec_error(client_rpc::query_authors_brief_info, this->client->last_error());
    }

    return client_rpc::query_authors_brief_info_returns(info_list);
}

QByteArray TangoThread::query_consumers_brief_info(int l, int r)
{
    std::vector<UserBriefInfo> info_list;
    if (!this->client->query_consumers_brief_info(info_list, l, r)) {
        return client_rpc::err_exec_error(client_rpc::query_consumers_brief_info, this->client->last_error());
    }

    return client_rpc::query_authors_brief_info_returns(info_list);
}

QByteArray TangoThread::query_authors_by_id(int id)
{
    UserFullInfo query_container;
    if (!this->client->query_authors_by_id(query_container, id)) {
        return client_rpc::err_exec_error(client_rpc::query_authors_by_id, this->client->last_error());
    }

    return client_rpc::query_users_info_returns(client_rpc::query_authors_by_id, query_container);
}

QByteArray TangoThread::query_authors_by_name(QString name)
{
    UserFullInfo query_container;
    if (!this->client->query_authors_by_name(query_container, name)) {
        return client_rpc::err_exec_error(client_rpc::query_authors_by_name, this->client->last_error());
    }

    return client_rpc::query_users_info_returns(client_rpc::query_authors_by_name, query_container);
}

QByteArray TangoThread::query_consumers_by_id(int id)
{
    UserFullInfo query_container;
    if (!this->client->query_consumers_by_id(query_container, id)) {
        return client_rpc::err_exec_error(client_rpc::query_authors_by_id, this->client->last_error());
    }

    return client_rpc::query_users_info_returns(client_rpc::query_authors_by_id, query_container);
}

QByteArray TangoThread::query_consumers_by_name(QString name)
{
    UserFullInfo query_container;
    if (!this->client->query_consumers_by_name(query_container, name)) {
        return client_rpc::err_exec_error(client_rpc::query_authors_by_name, this->client->last_error());
    }

    return client_rpc::query_users_info_returns(client_rpc::query_authors_by_name, query_container);
}


QByteArray TangoThread::query_users()
{
    int query_count;
    if (!this->client->query_users(query_count)) {
        return client_rpc::err_exec_error(client_rpc::query_users, this->client->last_error());
    }
    return client_rpc::query_users_returns(query_count);
}
