#ifndef CLIENTSLAVE_H
#define CLIENTSLAVE_H

#include <QObject>
#include <QThread>

class SocketX;

class TangoThread: public QThread
{
    Q_OBJECT
public:
    TangoThread(qintptr sockDesc, QObject *parent = nullptr);
    ~TangoThread();

private:
    void run(void);

signals:
    void disconnected_from_client(long long sockDesc);

private:
    SocketX *m_socket;
    long long client_id;
    void make_on_disconnected_client();
};

#endif // CLIENTSLAVE_H
