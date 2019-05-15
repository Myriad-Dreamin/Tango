
#include <functional>

#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QDebug>
#include "MainScene.h"


MainScene::MainScene(QWidget *parent): Scene(parent)
{
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
    buttons_lay->addWidget(sign_in_button);

    cancel_button = new QPushButton;
    cancel_button->setText("取消");
    buttons_lay->addWidget(cancel_button);

    sign_up_button = new QPushButton;
    sign_up_button->setText("注册");
    buttons_lay->addWidget(sign_up_button);

    buttons_lay->addStretch(1);

    auto main_center_lay = new QVBoxLayout;
    main_center_lay->addLayout(network_lay);
    main_center_lay->addLayout(account_lay);
    main_center_lay->addLayout(password_lay);
    main_center_lay->addLayout(buttons_lay);

    lay = new QGridLayout;
    lay->addLayout(main_center_lay, 1, 1);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(2, 1);
    lay->setRowStretch(2, 1);
    lay->setRowStretch(0, 1);

    setLayout(lay);
}

MainScene::~MainScene()
{
    qDebug() << "main scene deleted";
}

void MainScene::set_sign_in_button_event(std::function<void ()> ev)
{
    connect(sign_in_button, &QPushButton::clicked, ev);
}

void MainScene::set_sign_up_button_event(std::function<void ()> ev)
{
    connect(sign_up_button, &QPushButton::clicked, ev);
}

void MainScene::set_cancel_button_event(std::function<void ()> ev)
{
    connect(cancel_button, &QPushButton::clicked, ev);
}


