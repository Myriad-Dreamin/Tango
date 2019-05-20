#ifndef CLIENT_H
#define CLIENT_H

#include <functional>

#include <QObject>
#include <QHostAddress>
#include <QSqlDatabase>

#include "../types/UserStatus.h"
#include "../types/RetriveMode.h"

class QTcpSocket;
class QSqlDatabase;
class TangoPair;
class UserBriefInfo;
class UserFullInfo;
class Consumer;
class GameConfig;
class GameAutomation;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    virtual ~Client() override;

    bool setup_remote_connection(QHostAddress host_address, quint16 server_port);
    bool setup_local_connection();

    // TODO: check if mode is valid
    void switch_remote_mode();
    std::function<void ()> switch_remote_mode_slottor();
    void switch_local_mode();
    std::function<void ()> switch_local_mode_slottor();

    bool is_connected();
    bool is_remote_server_connected();
    bool is_local_handler_connected();

    bool author_sign_in(QString account, QString password);
    bool author_sign_up(QString account, QString password);
    bool consumer_sign_in(QString account, QString password);
    bool consumer_sign_up(QString account, QString password);
    bool logout();


    /*
    int user_id;
    QString name;

    int tango_count;
    int misson_count;
    */
    int consumer_exp();
    int consumer_level();

    bool submit_tango_items(const std::vector<TangoPair> &tango_list);
    GameAutomation *start_game_event(const GameConfig *game_config, int n, RetriveMode mode);
    bool settle_game_event(const GameAutomation *automate);
    bool query_authors_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r);
    bool query_consumers_brief_info(std::vector<UserBriefInfo> &info_list, int l, int r);

    bool query_authors_by_id(UserFullInfo &query_container, int id);
    bool query_authors_by_name(UserFullInfo &query_container, QString name);
    bool query_consumers_by_id(UserFullInfo &query_container, int id);
    bool query_consumers_by_name(UserFullInfo &query_container, QString name);

    const QString last_error();

private:

    UserStatus user_status;
    class Author *user_author;
    class Consumer *user_consumer;

    QHostAddress remote_address;
    quint16 remote_port;
    QTcpSocket *remote_server;

    QSqlDatabase local_handler;

    bool remote_ready;
    bool local_ready;
    QString _last_error;

    inline void make_remote_server_on_connected();
    inline void make_remote_server_on_disconnected();

    bool author_sign_in_local(QString account, QString password);
    bool author_sign_in_remote(QString account, QString password);

    bool author_sign_up_local(QString account, QString password);
    bool author_sign_up_remote(QString account, QString password);

    bool consumer_sign_in_local(QString account, QString password);
    bool consumer_sign_up_local(QString account, QString password);

    bool consumer_sign_in_remote(QString account, QString password);
    bool consumer_sign_up_remote(QString account, QString password);

    bool submit_tango_items_local(const std::vector<TangoPair> &tango_list);
    bool submit_tango_items_remote(const std::vector<TangoPair> &tango_list);

    bool logout_local();

    bool disconnect_to_remote();
    bool disconnect_to_local();

    bool create_tables();
    inline bool create_author_table();
    inline bool create_tangos_table();
    inline bool create_consumer_table();


    std::function<bool(QString,QString)> _author_sign_in;
    std::function<bool(QString,QString)> _author_sign_up;
    std::function<bool(QString,QString)> _consumer_sign_in;
    std::function<bool(QString,QString)> _consumer_sign_up;
    std::function<bool(const std::vector<TangoPair>&)> _submit_tango_items;
    std::function<bool()> _is_connected;

    GameAutomation *start_game_event_local(const GameConfig *game_config, int n, RetriveMode mode);
    bool retrive_tango_items_local(std::vector<TangoPair> &tango_list, int n, RetriveMode mode);
    int retrive_since_kth_tango_item_local(std::vector<TangoPair> &tango_list, unsigned int k, int n);
    bool retrive_kth_tango_item_local(TangoPair &tp, int k);
    bool settle_game_event_local(const GameAutomation *automate);
    bool settle_creation_event_local(const std::vector<TangoPair> &tango_list);

    bool query_authors_brief_info_local(std::vector<UserBriefInfo> &info_list, int l, int r);
    bool query_authors_by_id_local(UserFullInfo &query_container, int id);
    bool query_authors_by_name_local(UserFullInfo &query_container, QString name);

    bool query_consumers_brief_info_local(std::vector<UserBriefInfo> &info_list, int l, int r);
    bool query_consumers_by_id_local(UserFullInfo &query_container, int id);
    bool query_consumers_by_name_local(UserFullInfo &query_container, QString name);
signals:
    void connected();
    void disconnected();
};

#endif // CLIENT_H
