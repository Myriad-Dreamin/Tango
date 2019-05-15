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
class Consumer;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    bool setup_remote_connection(QHostAddress host_address, quint16 server_port);
    bool setup_local_connection();

    std::function<bool(QString,QString)> author_sign_in;
    std::function<bool(QString,QString)> author_sign_up;
    std::function<bool(QString,QString)> consumer_sign_in;
    std::function<bool(QString,QString)> consumer_sign_up;
    std::function<bool(std::vector<TangoPair>)> submit_tango_items;

    void switch_remote_mode();
    std::function<void ()> switch_remote_mode_slottor();
    void switch_local_mode();
    std::function<void ()> switch_local_mode_slottor();

    const QString last_error();
    bool is_remote_server_connected();
    bool is_local_handler_connected();
    bool retrive_tango_items(std::vector<TangoPair> &tango_list, int n, RetriveMode mode = RetriveMode::Easy);
private:
    UserStatus user_status;

    class Author *user_author;
    class Consumer *user_consumer;

    QHostAddress remote_address;
    quint16 remote_port;

    QTcpSocket *remote_server;
    QSqlDatabase local_handler;

    QString _last_error;
    bool remote_ready;
    bool local_ready;
    inline void make_remote_server_on_connected();
    inline void make_remote_server_on_disconnected();

    bool author_sign_in_local(QString account, QString password);
    bool author_sign_in_remote(QString account, QString password);
    bool author_sign_up_local(QString account, QString password);
    bool author_sign_up_remote(QString account, QString password);
    bool disconnect_to_remote();
    bool disconnect_to_local();
    bool create_tables();
    inline bool create_author_table();
    inline bool create_tangos_table();
    bool submit_tango_items_local(const std::vector<TangoPair> &tango_list);
    bool submit_tango_items_remote(const std::vector<TangoPair> &tango_list);
    bool create_consumer_table();
    bool consumer_sign_in_local(QString account, QString password);
    bool consumer_sign_up_local(QString account, QString password);
    bool consumer_sign_in_remote(QString account, QString password);
    bool consumer_sign_up_remote(QString account, QString password);
    bool retrive_kth_tango_item(TangoPair &tp, int k);
    int retrive_since_kth_tango_item(std::vector<TangoPair> &tango_list, unsigned int k, int n);
signals:
    void connected();
    void disconnected();
};

#endif // CLIENT_H
