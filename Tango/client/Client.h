#ifndef CLIENT_H
#define CLIENT_H

#include <functional>
#include <QObject>
#include <QHostAddress>
#include <QSqlDatabase>

class QTcpSocket;
class QSqlDatabase;
class TangoPair;

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
    std::function<bool(std::vector<TangoPair>)> submit_tango_items;

    void switch_remote_mode();
    std::function<void ()> switch_remote_mode_slottor();
    void switch_local_mode();
    std::function<void ()> switch_local_mode_slottor();

    const QString last_error();
    bool is_remote_server_connected();
    bool is_local_handler_connected();
private:
    enum UserStatusType: unsigned char
    {
        None      = 0x0 | 0x0,
        Author    = 0x1 | 0x0,
        Consumer  = 0x0 | 0x2,
        Both      = 0x1 | 0x2
    } user_status;

    class Author *user_author;

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
signals:
    void connected();
    void disconnected();
};

#endif // CLIENT_H
