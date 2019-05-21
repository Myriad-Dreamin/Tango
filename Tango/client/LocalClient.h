#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include "AbstractClient.h"
#include <QObject>
#include <QSqlDatabase>


class LocalClient : public QObject, public AbstractClient
{
    Q_OBJECT
public:
    explicit LocalClient(QObject *parent = nullptr);
    virtual ~LocalClient() override;
signals:
    /* 连接信号 */
    void connected();
    /* 断开信号 */
    void disconnected();

    // AbstractClient interface
public:

    bool setup_connection() override;

    bool author_sign_in(QString account, QString password) override;
    bool author_sign_up(QString account, QString password) override;
    bool consumer_sign_in(QString account, QString password) override;
    bool consumer_sign_up(QString account, QString password) override;

    bool logout() override;
    bool sync_status() override;

    bool init_default_tangos() override;

    bool submit_tango_items(const std::vector<TangoPair> &tango_list) override;

    GameAutomation *start_game_event(const GameConfig *game_config, int n, RetriveMode mode) override;
    bool settle_game_event(const GameAutomation *automate) override;

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

    const UserFullInfo consumer_info() override;
    const UserFullInfo author_info() override;
private:
    /* 用户状态 */
    UserStatus user_status;

    /* author用户handler */
    class Author *user_author;
    /* consumer用户handler */
    class Consumer *user_consumer;

    /* 本地连接handler */
    QSqlDatabase handler;
    bool ready;

    QString _last_error;

    bool create_tables();
    bool create_author_table();
    bool create_consumer_table();
    bool create_tangos_table();

    bool _author_sign_in(QString account, QString password);
    bool _author_sign_up(QString account, QString password);
    bool _consumer_sign_in(QString account, QString password);
    bool _consumer_sign_up(QString account, QString password);
    bool _submit_tango_items(const std::vector<TangoPair> &tango_list);
    bool settle_creation_event(const std::vector<TangoPair> &tango_list);
    GameAutomation *_start_game_event(const GameConfig *game_config, int n, RetriveMode mode);
    bool retrive_tango_items(std::vector<TangoPair> &tango_list, int &n, RetriveMode mode);
    int retrive_since_kth_tango_item(std::vector<TangoPair> &tango_list, unsigned int k, int n);
    bool retrive_kth_tango_item(TangoPair &tp, int k);
};

#endif // LOCALCLIENT_H
