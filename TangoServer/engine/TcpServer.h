

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QSqlDatabase>

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
signals:
    void client_disconnected(qintptr sockDesc);

private:
    void incomingConnection(qintptr sockDesc);
    void make_on_client_disconnected(TangoThread *thread);
    std::map<qintptr, TangoThread*> active_threads;
    MainWindow *main_window;
    QSqlDatabase tango_sql;

};

#endif // TCPSERVER_H
