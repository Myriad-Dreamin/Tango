
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QAction>

#include <QHostAddress>

// toolbar, statusbar

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    // ui(new Ui::MainWindow)
{
    qDebug() << "I...";

    this->cur_scene = nullptr;

    this->init_client();
    this->init_main_scene();
    this->init_register_scene();

    auto main_menubar = menuBar();
    auto menu_item = main_menubar->addMenu("File(&F)");

    menu_item->addAction("&Open", this, SLOT(close()), QKeySequence::Open);
    menu_item->setGeometry(0,0,this->width(),30);
    main_menubar->addMenu(menu_item);

    this->switch_scene(main_scene);


    // hlay->setParent(&w);
    this->setMinimumSize(800, 600);
    this->setWindowTitle("Tango!");
}


MainWindow::~MainWindow()
{
    // delete ui;
}


bool MainWindow::init_main_scene()
{
    main_scene = new MainScene;
    main_scene->set_sign_in_button_event([this]() mutable {

        QString account_text = this->main_scene->account_edit->text();
        QString password_text = this->main_scene->password_edit->text();
        qDebug() << "clicked confirm button" << account_text;
        qDebug() << "clicked confirm button" << password_text;


        if (account_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("用户名不能为空"), QMessageBox::Ok);
            return;
        }

        if (password_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("密码不能为空"), QMessageBox::Ok);
            return;
        }


        QHostAddress host_address(this->main_scene->network_edit->text());
        quint16 server_port = quint16(this->main_scene->port_edit->text().toShort());

        this->try_connect_to_server(host_address, server_port);

        if (this->client->waitForConnected(1000) == false) {
            QMessageBox::critical(this, tr("错误"), tr("服务器连接失败"), QMessageBox::Ok);
            return;
        }

        QByteArray qbytes;
        qbytes.append(account_text);
        client->write(qbytes);

        qbytes.clear();
        qbytes.append(password_text);
        client->write(qbytes);
    });

    main_scene->set_cancel_button_event([this]() mutable {
        qDebug() << "clicked cancel button";
        this->close();
    });
    main_scene->set_sign_up_button_event([this]() mutable {
         this->switch_scene(this->register_scene);
    });

    return true;
}


bool MainWindow::init_register_scene()
{
    register_scene = new RegisterScene;

    register_scene->set_confirm_button_event([this]() mutable {
        QString account_text = this->register_scene->account_edit->text();
        QString password_text = this->register_scene->password_edit->text();
        QString confirm_password_text = this->register_scene->confirm_edit->text();
        qDebug() << "clicked confirm button" << account_text;
        qDebug() << "clicked confirm button" << password_text;
        qDebug() << "clicked confirm button" << confirm_password_text;

        if (account_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("用户名不能为空"), QMessageBox::Ok);
        }

        if (password_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("密码不能为空"), QMessageBox::Ok);
        }

        if (password_text != confirm_password_text) {
            qDebug() << "not equal";
            QMessageBox::critical(this, tr("错误"), tr("两次输入密码不一致"), QMessageBox::Ok);
        }

    });

    register_scene->set_cancel_button_event([this]() mutable {
        qDebug() << "clicked cancel button";
        this->close();
    });

    register_scene->set_return_button_event([this]() mutable {
        qDebug() << "clicked return button" << this->main_scene;
        this->switch_scene(this->main_scene);
    });

    return true;
}

void MainWindow::switch_scene(QWidget *to_set)
{
    qDebug() << this->cur_scene << to_set;
    if (this->cur_scene) {
        this->cur_scene->hide();
    }
    this->takeCentralWidget();
    this->setCentralWidget(to_set);
    this->cur_scene = to_set;
    this->cur_scene->show();
    return;
}


/************************************* NetWork *************************************/


bool MainWindow::try_connect_to_server(QHostAddress host_address, quint16 server_port)
{
    if (this->client_connected == false) {
        client->connectToHost(host_address, server_port);
        this->connected_server_address = host_address;
        this->connected_server_port = server_port;
        return true;
    }

    if (this->connected_server_address != host_address || this->connected_server_port != server_port) {
        client->disconnectFromHost();
        client->connectToHost(host_address, server_port);
        this->connected_server_address = host_address;
        this->connected_server_port = server_port;
        return true;
    }
    return true;
}


bool MainWindow::init_client()
{
    this->client = new QTcpSocket;
    this->client_connected = false;

    connect(this->client, SIGNAL(connected()), this, SLOT(client_on_connected()));
    connect(this->client, SIGNAL(disconnected()), this, SLOT(client_on_closed()));

    return true;
}


void MainWindow::client_on_connected()
{
    qDebug() << "client_on_connected";
    this->client_connected = true;
}
void MainWindow::client_on_closed()
{
    qDebug() << "client_on_closed";
    this->client_connected = false;
}


void MainWindow::makeLayout()
{
}

