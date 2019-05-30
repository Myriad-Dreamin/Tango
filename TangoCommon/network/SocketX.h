#ifndef SOCKETX_H
#define SOCKETX_H


#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>

const unsigned char package_delimit = 0x01;

class QEventLoop;

/* self defined protocal based on QTcpSocket */
/* 基于QTcpSocket自定义协议 */
class SocketX : public QTcpSocket
{
    Q_OBJECT
public:
    SocketX(qintptr sockDesc, QObject *parent = nullptr);
    ~SocketX();

// c++11 stype public lambda slot:
    std::function<void (QByteArray)> write_slotter();

    bool write_package(QByteArray to_send);
    bool wait_for_new_package(const std::function<void (QByteArray)> &callback, int timeout=1000);
signals:
    void package_ready(const QString &ip, const quint16 &port, const QByteArray &data);
    void package_isready();
private:
    long long sock_id;
    QByteArray buffer;

    void make_ready_read_slot(void);
};

#endif // SOCKETX_H
