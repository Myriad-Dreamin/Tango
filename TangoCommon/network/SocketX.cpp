
#include <functional>
#include <QTimer>
#include <QEventLoop>
#include <QTimerEvent>
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

bool SocketX::wait_for_new_package(const std::function<void (QByteArray)> &callback, int timeout)
{
    bool success = false;
    QTimer loop_timeout;
    loop_timeout.setSingleShot(true);
    QEventLoop loop;
    connect(&loop_timeout, &QTimer::timeout, &loop, &QEventLoop::quit);

    auto xx =  [&](const QString& ip,const quint16& port,QByteArray bytes_json) mutable {
        qDebug() << ip << port << bytes_json;
        loop_timeout.stop();
        callback(bytes_json);
        success = true;
        loop.exit();
    };
    connect(this, &SocketX::package_ready, xx);
    loop_timeout.start(timeout);
    loop.exec();
    disconnect(this, &SocketX::package_ready, this, nullptr);
    return success;
}

bool SocketX::write_package(QByteArray to_send)
{
    this->write(to_send.toBase64().append(package_delimit));
    return true;
}

void SocketX::make_ready_read_slot(void)
{
    connect(this, &QIODevice::readyRead, [this]() mutable {
        qDebug() << "new Socket X Data from " << this->peerName() << " " << this->peerAddress() << " " << this->peerPort();
        QByteArray data = readAll();
        qDebug() << "retriving data" << data;

        auto package_list = data.split(package_delimit);
        buffer.append(package_list[0]);
        qDebug() << "retriving size" << package_list.length();
        int packages_count = package_list.length() - 1;
        qDebug() << "retriving size" << package_list;
        qDebug() << "retrived size" << packages_count;
        if (package_list.length() > 1) {
            qDebug() << "retriving size" << QByteArray::fromBase64(buffer);
            emit this->package_ready(peerAddress().toString(), this->peerPort(), QByteArray::fromBase64(buffer));
            buffer = package_list[packages_count];
        }
        qDebug() << "retriving size" << buffer;
        for (int i = 1; i < packages_count; i++) {
            emit this->package_ready(peerAddress().toString(), this->peerPort(), QByteArray::fromBase64(package_list[i]));
        }
    });
}
