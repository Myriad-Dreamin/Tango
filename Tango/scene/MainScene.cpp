
#include "MainScene.h"

#include <functional>

#include <QDebug>

#include <QLayout>

#include <QLabel>
#include <QLineEdit>

#include <QPushButton>
#include <QRadioButton>

#include "../mainwindow.h"

MainScene::MainScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);
    this->user_selecting_status = UserStatus::Author;

    auto network_lay = new QHBoxLayout;
    auto network_host_lab = new QLabel("服务器地址: ");
    auto network_port_lab = new QLabel("端口: ");
    remote_button = new QRadioButton("远程连接");
    network_edit = new QLineEdit;
    port_edit = new QLineEdit;
    network_lay->addWidget(remote_button);
    network_lay->addWidget(network_host_lab);
    network_lay->addWidget(network_edit, 1);
    network_lay->addWidget(network_port_lab);
    network_lay->addWidget(port_edit, 1);

    auto role_lay = new QHBoxLayout;
    role_button = new QPushButton("author!");
    role_lay->addWidget(role_button, 1);
    role_button->setMinimumSize(400, 32);

    auto account_lay = new QHBoxLayout;
    auto account_lab = new QLabel("账户: ");
    account_edit = new QLineEdit;
    account_lay->addWidget(account_lab);
    account_lay->addWidget(account_edit, 1);

    auto password_lay = new QHBoxLayout;
    auto password_lab = new QLabel("密码: ");
    password_edit = new QLineEdit;
    password_lay->addWidget(password_lab);
    password_lay->addWidget(password_edit, 1);

    auto buttons_lay = new QHBoxLayout;
    buttons_lay->addStretch(1);

    sign_in_button = new QPushButton;
    sign_in_button->setText("登录");
    sign_in_button->setMinimumHeight(32);
    buttons_lay->addWidget(sign_in_button, 3);

    buttons_lay->addStretch(1);
    cancel_button = new QPushButton;
    cancel_button->setText("取消");
    cancel_button->setMinimumHeight(32);
    buttons_lay->addWidget(cancel_button, 3);

    buttons_lay->addStretch(1);

    sign_up_button = new QPushButton;
    sign_up_button->setText("注册");
    sign_up_button->setMinimumHeight(32);
    buttons_lay->addWidget(sign_up_button, 3);

    buttons_lay->addStretch(1);

    auto main_center_lay = new QVBoxLayout;
    main_center_lay->addStretch(20);
    main_center_lay->addLayout(network_lay, 10);
    main_center_lay->addStretch(1);
    main_center_lay->addLayout(role_lay, 10);
    main_center_lay->addStretch(1);
    main_center_lay->addLayout(account_lay, 10);
    main_center_lay->addStretch(1);
    main_center_lay->addLayout(password_lay, 10);
    main_center_lay->addStretch(1);
    main_center_lay->addLayout(buttons_lay, 10);
    main_center_lay->addStretch(20);


    lay = new QHBoxLayout;
    lay->addStretch(2);
    lay->addLayout(main_center_lay, 7);
    lay->addStretch(2);

    setLayout(lay);
    this->setMinimumSize(400, 400);
}

MainScene::~MainScene()
{
    qDebug() << "main scene deleted";
}


void MainScene::set_sign_in_button_event(const std::function<void ()> &ev)
{
    connect(sign_in_button, &QPushButton::clicked, ev);
}

void MainScene::set_sign_up_button_event(const std::function<void ()> &ev)
{
    connect(sign_up_button, &QPushButton::clicked, ev);
}

void MainScene::set_cancel_button_event(const std::function<void ()> &ev)
{
    connect(cancel_button, &QPushButton::clicked, ev);
}

void MainScene::set_role_button_event(const std::function<void ()> &ev)
{
    connect(role_button, &QPushButton::clicked, ev);
}
