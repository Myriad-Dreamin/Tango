

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QSqlDatabase>

class MainWindow;
class TangoThread;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    explicit TcpServer(QSqlDatabase &out_link, QObject *parent = nullptr);
    ~TcpServer();


signals:
    void client_disconnected(qintptr sockDesc);

private:
    void incomingConnection(qintptr sockDesc);
    void make_on_client_disconnected(TangoThread *thread);
    MainWindow *main_window;
    QSqlDatabase tango_sql;

};

#endif // TCPSERVER_H
