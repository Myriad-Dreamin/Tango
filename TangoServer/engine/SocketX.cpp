
#include <functional>
#include "SocketX.h"

SocketX::SocketX(qintptr sock_desc, QObject *parent) :
    QTcpSocket(parent),
    sock_id(sock_desc)
{
    this->make_ready_read_slot();
}

SocketX::~SocketX() {}

std::function<void(QByteArray)> SocketX::write_slotter()
{
    static auto writer_to_socket = [&](QByteArray data) mutable {
        this->write(data);
    };
    return writer_to_socket;
}

void SocketX::make_ready_read_slot(void)
{
    connect(this, &QIODevice::readyRead, [this]() mutable {
        qDebug() << "new Socket X Data from " << this->peerName() << " " << this->peerAddress() << " " << this->peerPort();
        QByteArray data = readAll();
        qDebug() << "retriving data" << data;
        emit this->data_ready(peerAddress().toString().remove(0, 7), data);
    });
}
