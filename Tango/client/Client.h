#ifndef CLIENT_H
#define CLIENT_H

#include "GameClient.h"

#include <functional>

#include <QObject>
#include <QHostAddress>
#include <QSqlDatabase>

#include "../types/UserStatus.h"
#include "../types/RetriveMode.h"

class QTcpSocket;
class QSqlDatabase;
class TangoPair;
class Consumer;

class Client : public QObject, public GameClient
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

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
    bool submit_tango_items(const std::vector<TangoPair> &tango_list);

    bool retrive_tango_items(std::vector<TangoPair> &tango_list, int n, RetriveMode mode = RetriveMode::Easy);

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

    bool disconnect_to_remote();
    bool disconnect_to_local();

    bool create_tables();
    inline bool create_author_table();
    inline bool create_tangos_table();
    inline bool create_consumer_table();

    bool retrive_kth_tango_item(TangoPair &tp, int k);
    int retrive_since_kth_tango_item(std::vector<TangoPair> &tango_list, unsigned int k, int n);

    std::function<bool(QString,QString)> _author_sign_in;
    std::function<bool(QString,QString)> _author_sign_up;
    std::function<bool(QString,QString)> _consumer_sign_in;
    std::function<bool(QString,QString)> _consumer_sign_up;
    std::function<bool(const std::vector<TangoPair>&)> _submit_tango_items;
    std::function<bool()> _is_connected;

signals:
    void connected();
    void disconnected();
};

#endif // CLIENT_H
