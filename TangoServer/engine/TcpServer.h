

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QSqlDatabase>
#include <QMutex>
#include <unordered_map>

class MainWindow;
class TangoThread;
class UserFullInfo;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    explicit TcpServer(QSqlDatabase &out_link, QObject *parent = nullptr);
    ~TcpServer();


    void query_online_threads(std::vector<UserFullInfo> &authors_info, std::vector<UserFullInfo> &consumers_info, std::vector<long long> &socks);
    bool author_pool_register(QString account, TangoThread *thread);
    bool author_pool_unregister(QString account, TangoThread *thread);
    bool consumer_pool_register(QString account, TangoThread *thread);
    bool consumer_pool_unregister(QString account, TangoThread *thread);
signals:
    void client_disconnected(qintptr sockDesc);

private:
    void incomingConnection(qintptr sockDesc);
    void make_on_client_disconnected(TangoThread *thread);
    std::map<qintptr, TangoThread*> active_threads;
    std::unordered_map<std::string, TangoThread*> online_author, online_consumer;
    MainWindow *main_window;
    QSqlDatabase tango_sql;
    QMutex m_mutex;
};

#endif // TCPSERVER_H
