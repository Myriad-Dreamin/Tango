
#include "TcpServer.h"
#include "TangoThread.h"
#include "../mainwindow.h"
#include "../TangoCommon/network/SocketX.h"
#include "../TangoCommon/client/LocalClient.h"
#include "../TangoCommon/types/UserFullInfo.h"


inline void gracefully_destroy_thread(QThread *thread)
{
    thread->quit();
    thread->wait();
    thread->deleteLater();
}

TcpServer::TcpServer(QObject *parent): QTcpServer (parent)
{
    main_window = dynamic_cast<MainWindow*>(parent);
    active_threads.clear();
    // connect()
}

TcpServer::TcpServer(QSqlDatabase &out_link, QObject *parent): QTcpServer (parent)
{
    main_window = dynamic_cast<MainWindow*>(parent);
    tango_sql = out_link;
    active_threads.clear();
    // connect()
}

TcpServer::~TcpServer() {
    for (auto thread: active_threads) {
        gracefully_destroy_thread(thread.second);
    }
}

void TcpServer::incomingConnection(qintptr sock_desc)
{
    qDebug() << "incoming connection" << sock_desc;

    TangoThread *thread = new TangoThread(sock_desc, tango_sql, this);

    this->make_on_client_disconnected(thread);

    active_threads[sock_desc] = thread;
    qDebug() << "moving" << "-----------";
    thread->m_socket->moveToThread(thread);
    qDebug() << "moving" << "-----------";
    thread->client->moveToThread(thread);
    qDebug() << "moving" << "-----------";

//    connect(thread, SIGNAL(dataReady(const QString&, const QByteArray&)),
//            m_dialog, SLOT(recvData(const QString&, const QByteArray&)));

//    connect(m_dialog, SIGNAL(sendData(int, const QByteArray&)),
//            thread, SLOT(sendDataSlot(int, const QByteArray&)));

    thread->start();
}

void TcpServer::query_online_threads(
    std::vector<UserFullInfo> &authors_info,
    std::vector<UserFullInfo> &consumers_info,
    std::vector<long long> &socks
) {
    for (auto thread: active_threads) {
        socks.push_back(thread.first);
        authors_info.push_back(thread.second->author_info());
        consumers_info.push_back(thread.second->consumer_info());
    }

    UserFullInfo info = UserFullInfo();
    info.user_id = 1;
    info.name = "假人";
    info.level = 666;
    for (int i = 0; i < 20; i++) {
        socks.push_back(-1);
        consumers_info.push_back(UserFullInfo());
        authors_info.push_back(info);
    }
}

void TcpServer::make_on_client_disconnected(TangoThread *thread)
{
    connect(thread, &TangoThread::disconnected_from_client, this, [this, thread](long long sock_desc) {
        qDebug() << "on client disconnected " << sock_desc;
        active_threads.erase(sock_desc);
        gracefully_destroy_thread(thread);
        emit client_disconnected(sock_desc);
    });
}


bool TcpServer::author_pool_register(TangoThread *thread)
{
    m_mutex.lock();
    if (online_author.count(thread->last_author_info().name.toStdString())) {
        m_mutex.unlock();
        thread->_last_error = "author is logining..";
        return false;
    }
    online_author[thread->last_author_info().name.toStdString()] = thread;
    m_mutex.unlock();
    return true;
}

bool TcpServer::author_pool_unregister(TangoThread *thread)
{
    m_mutex.lock();
    if (!online_author.count(thread->last_author_info().name.toStdString())) {
        m_mutex.unlock();
        thread->_last_error = "author is not logining..";
        return false;
    }
    online_author.erase(thread->last_author_info().name.toStdString());
    m_mutex.unlock();
    return true;
}

bool TcpServer::consumer_pool_register(TangoThread *thread)
{
    m_mutex.lock();
    qDebug() << "thread->consumer_info().name.toStdString()" << thread->last_consumer_info().name;
    if (online_consumer.count(thread->last_consumer_info().name.toStdString())) {
        m_mutex.unlock();
        thread->_last_error = "consumer is logining..";
        return false;
    }
    online_consumer[thread->last_consumer_info().name.toStdString()] = thread;
    m_mutex.unlock();
    return true;
}

bool TcpServer::consumer_pool_unregister(TangoThread *thread)
{
    m_mutex.lock();
    qDebug() << "thread->consumer_info().name.toStdString()" << thread->last_consumer_info().name;
    if (!online_consumer.count(thread->last_consumer_info().name.toStdString())) {
        m_mutex.unlock();
        thread->_last_error = "consumer is not logining..";
        return false;
    }
    online_consumer.erase(thread->last_consumer_info().name.toStdString());
    m_mutex.unlock();
    return true;
}
