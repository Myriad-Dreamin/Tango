

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class MainWindow;
class TangoThread;

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();


signals:
    void client_disconnected(qintptr sockDesc);

private:
    void incomingConnection(qintptr sockDesc);
    void make_on_client_disconnected(TangoThread *thread);
    MainWindow *main_window;

};

#endif // TCPSERVER_H
