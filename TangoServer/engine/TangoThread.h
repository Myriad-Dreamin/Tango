#ifndef CLIENTSLAVE_H
#define CLIENTSLAVE_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>

class SocketX;
class LocalClient;
class TangoPair;
class RetriveMode;

class TangoThread: public QThread
{
    Q_OBJECT
public:
    TangoThread(qintptr sockDesc, QObject *parent = nullptr);
    TangoThread(qintptr sockDesc, QSqlDatabase &tango_sql, QObject *parent = nullptr);
    ~TangoThread();

private:
    void run(void);

signals:
    void disconnected_from_client(long long sockDesc);

private:
    SocketX *m_socket;
    LocalClient *client;
    long long client_id;
    void make_on_disconnected_client();
    QByteArray author_sign_in(QString account, QString password);
    QByteArray author_sign_up(QString account, QString password);
    QByteArray consumer_sign_in(QString account, QString password);
    QByteArray consumer_sign_up(QString account, QString password);
    QByteArray logout();
    QByteArray sync_status();
    QByteArray submit_tango_items(std::vector<TangoPair> &tango_list);
    QByteArray start_game_event(const int game_config_id, int n, RetriveMode mode);
};

#endif // CLIENTSLAVE_H
