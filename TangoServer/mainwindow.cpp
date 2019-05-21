

#include "mainwindow.h"
#include "engine/TcpServer.h"

#include <QLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    server = new TcpServer(this);
    server->listen(QHostAddress::AnyIPv4,8888);
    this->make_layers();
    // connect(server, &QTcpServer::newConnection,this,&MainWindow::newCon);
}

MainWindow::MainWindow(QSqlDatabase &out_link, QWidget *parent)
    : QMainWindow(parent)
{
    server = new TcpServer(out_link, this);
    server->listen(QHostAddress::AnyIPv4,8888);
    this->make_layers();
}

void MainWindow::make_layers()
{

    connect(server, &TcpServer::client_disconnected, [this](qintptr sockDesc) mutable {
        this->text_edit->setText(QString::number(sockDesc));
    });

    auto text_lay = new QHBoxLayout;
    auto text_lab = new QLabel("text");
    text_edit = new QTextEdit;
    text_lay->addStretch(1);
    text_lay->addWidget(text_lab, 1);
    text_lay->addWidget(text_edit);
    text_lay->addStretch(1);

    auto address_lay = new QHBoxLayout;
    auto address_lab = new QLabel("address");
    address_edit = new QTextEdit;
    address_lay->addStretch(1);
    address_lay->addWidget(address_lab, 1);
    address_lay->addWidget(address_edit);
    address_lay->addStretch(1);

    auto port_lay = new QHBoxLayout;
    auto port_lab = new QLabel("port");
    port_edit = new QTextEdit;
    port_lay->addStretch(1);
    port_lay->addWidget(port_lab, 1);
    port_lay->addWidget(port_edit);
    port_lay->addStretch(1);

    auto center_lay = new QVBoxLayout;
    center_lay->addLayout(text_lay);
    center_lay->addLayout(address_lay);
    center_lay->addLayout(port_lay);

    auto main_lay = new QGridLayout;

    main_lay->setRowStretch(0, 1);
    main_lay->addLayout(center_lay, 1, 1);
    main_lay->setRowStretch(2, 1);
    main_lay->setColumnStretch(0, 1);
    main_lay->setColumnStretch(2, 1);

    auto main_wid = new QWidget;
    main_wid->setLayout(main_lay);

    this->setCentralWidget(main_wid);
}

MainWindow::~MainWindow()
{

}

#ifdef JOIFJAISJFOJ
void MainWindow::newCon()
{
    qDebug() << "data snew is:";
    QTcpSocket *socket =server->nextPendingConnection(); //创建socket连接
    QHostAddress clientAddress = socket->peerAddress();
    quint16 clientPort=socket->peerPort();

    address_edit->setText(clientAddress.toString());
    port_edit->setText(QString::number(clientPort));

    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::dealData);
}
#endif

void MainWindow::dealData()
{
    QTcpSocket *socket=dynamic_cast<QTcpSocket*>(sender());  //通过该函数获得信号的发出者

    qDebug() << "data sender socket is:" << reinterpret_cast<unsigned long long>(socket);

    QString data = QString(socket->readAll());
    qDebug() << data;
    text_edit->setText(data);
}
