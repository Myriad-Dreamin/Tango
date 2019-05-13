#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    server = new QTcpServer;
    server->listen(QHostAddress::AnyIPv4,8888);

    text_edit = new QTextEdit;
    address_edit = new QTextEdit;
    port_edit = new QTextEdit;
    connect(server,&QTcpServer::newConnection,this,&MainWindow::newCon);
}

MainWindow::~MainWindow()
{

}

void MainWindow::newCon()
{
    QTcpSocket *socket =server->nextPendingConnection(); //创建socket连接
    QHostAddress clientAddress = socket->peerAddress();
    quint16 clientPort=socket->peerPort();

    address_edit->setText(clientAddress.toString());
    port_edit->setText(QString::number(clientPort));

    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::dealData);
}


void MainWindow::dealData()
{
    QTcpSocket *socket=dynamic_cast<QTcpSocket*>(sender());  //通过该函数获得信号的发出者
    text_edit->setText(QString(socket->readAll()));
}
