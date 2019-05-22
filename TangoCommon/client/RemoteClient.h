#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H

#include "AbstractClient.h"
#include <QObject>
#include <QSqlDatabase>
#include "../Tango/mainwindow.h"
class SocketX;
class GameAutomationRelayer;
class MainWindow;

class RemoteClient : public QObject, public AbstractClient
{
    Q_OBJECT
    friend class GameAutomationRelayer;
public:
    explicit RemoteClient(QObject *parent = nullptr);
    virtual ~RemoteClient() override;
signals:
    /* 连接信号 */
    void connected();
    /* 断开信号 */
    void disconnected();

    void game_signal_start_game();
    void game_signal_elasped();
    void game_signal_new_tango(TangoPair,int);
    void game_signal_tango_faded(int);
    void game_signal_answer_failed();
    void game_signal_success();
    void game_signal_failed();

    // AbstractClient interface
public:

    bool setup_connection(QHostAddress host_address, quint16 server_port);
    bool stop_connection();

    bool author_sign_in(QString account, QString password) override;
    bool author_sign_up(QString account, QString password) override;
    bool consumer_sign_in(QString account, QString password) override;
    bool consumer_sign_up(QString account, QString password) override;

    bool logout() override;
    bool sync_status() override;

    bool submit_tango_items(const std::vector<TangoPair> &tango_list) override;

    AbstractGameAutomation *start_game_event(const GameConfig *game_config, int n, RetriveMode mode) override;
    bool settle_game_event(const AbstractGameAutomation *automate) override;

    bool query_authors_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r) override;
    bool query_consumers_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r) override;
    bool query_authors_by_id(UserFullInfo &query_container, int id) override;
    bool query_authors_by_name(UserFullInfo &query_container, QString name) override;
    bool query_consumers_by_id(UserFullInfo &query_container, int id) override;
    bool query_consumers_by_name(UserFullInfo &query_container, QString name) override;

    bool query_users(int &query_count) override;

    const QString last_error() override;

    bool is_connected() override;

    bool consumer_logining() override;
    bool author_logining() override;

    int consumer_exp() override;
    int consumer_level() override;

    const UserFullInfo &consumer_info() override;
    const UserFullInfo &author_info() override;

    bool create_tables();
private:
    MainWindow *parent;
    /* 用户状态 */
    UserStatus user_status;

    /* author用户handler */
    class Author *user_author;
    /* consumer用户handler */
    class Consumer *user_consumer;

    bool ready;
    /* 远程连接地址 */
    QHostAddress remote_address;
    /* 远程连接端口 */
    quint16 remote_port;
    /* 远程连接handler */
    SocketX *handler;

    QString current_key;
    QString _last_error;
    void make_server_on_connected();
    void make_server_on_disconnected();
    void game_stop();
    void game_answer(QString tango);
    void game_start();
    void receive_packages(const QString &ip, const quint16 &port, const QByteArray &data);
    void params_packages(int id, QJsonArray params);
    void returns_packages(int id, QJsonValue rets, QString err);
};

#endif // REMOTECLIENT_H
