
#include "TcpServer.h"
#include "TangoThread.h"
#include "../mainwindow.h"


TcpServer::TcpServer(QObject *parent): QTcpServer (parent)
{
    main_window = dynamic_cast<MainWindow*>(parent);
    // connect()
}

TcpServer::TcpServer(QSqlDatabase &out_link, QObject *parent): QTcpServer (parent)
{
    main_window = dynamic_cast<MainWindow*>(parent);
    tango_sql = out_link;
    // connect()
}

TcpServer::~TcpServer() {}

void TcpServer::incomingConnection(qintptr sockDesc)
{
    qDebug() << "incoming connection" << sockDesc;

    TangoThread *thread = new TangoThread(sockDesc, tango_sql);

    this->make_on_client_disconnected(thread);


//    connect(thread, SIGNAL(dataReady(const QString&, const QByteArray&)),
//            m_dialog, SLOT(recvData(const QString&, const QByteArray&)));

//    connect(m_dialog, SIGNAL(sendData(int, const QByteArray&)),
//            thread, SLOT(sendDataSlot(int, const QByteArray&)));

    thread->start();
}



void TcpServer::make_on_client_disconnected(TangoThread *thread)
{
    connect(thread, &TangoThread::disconnected_from_client, this, [this, thread](long long sock_desc) {
        qDebug() << "on client disconnected " << sock_desc;
        thread->quit();
        thread->wait();
        thread->deleteLater();
        emit client_disconnected(sock_desc);
    });
}
