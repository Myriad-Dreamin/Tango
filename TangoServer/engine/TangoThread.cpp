

#include "TangoThread.h"
#include "SocketX.h"

TangoThread::TangoThread(long long sock_desc, QObject *parent) :
    QThread(parent),
    client_id(sock_desc)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

TangoThread::~TangoThread()
{
    m_socket->close();
}

void TangoThread::run(void)
{
    m_socket = new SocketX(client_id);

    if (!m_socket->setSocketDescriptor(client_id)) {
        return ;
    }

    this->make_on_disconnected_client();
    this->exec();
}

void TangoThread::make_on_disconnected_client()
{
    connect(m_socket, &SocketX::disconnected, [this]() {
        qDebug() << "disconnected Client Server" << this->client_id;
        emit disconnected_from_client(this->client_id);
        m_socket->disconnectFromHost();
    });
}
