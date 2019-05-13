#ifndef SOCKETX_H
#define SOCKETX_H


#include <QTcpSocket>
#include <QHostAddress>


/* self defined protocal based on QTcpSocket */
/* 基于QTcpSocket自定义协议 */
class SocketX : public QTcpSocket
{
    Q_OBJECT
public:
    SocketX(qintptr sockDesc, QObject *parent = Q_NULLPTR);
    ~SocketX();

// c++ stype public lambda slot:
    std::function<void (QByteArray)> write_slotter();
signals:
    void data_ready(const QString &ip, const QByteArray &data);

private:
    long long sock_id;

    void make_ready_read_slot(void);
};

#endif // SOCKETX_H
