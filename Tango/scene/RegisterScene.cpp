

#include <functional>

#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>

#include "RegisterScene.h"
#include "../mainwindow.h"

RegisterScene::RegisterScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);
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

    auto confirm_lay = new QHBoxLayout;
    auto confirm_lab = new QLabel("确认: ");
    confirm_edit = new QLineEdit;
    confirm_lay->addWidget(confirm_lab);
    confirm_lay->addWidget(confirm_edit, 1);

    auto buttons_lay = new QHBoxLayout;

    buttons_lay->addStretch(1);

    confirm_button = new QPushButton;
    confirm_button->setText("确认");
    buttons_lay->addWidget(confirm_button);

    cancel_button = new QPushButton;
    cancel_button->setText("取消");
    buttons_lay->addWidget(cancel_button);

    return_button = new QPushButton;
    return_button->setText("返回");
    buttons_lay->addWidget(return_button);

    buttons_lay->addStretch(1);

    auto main_center_lay = new QVBoxLayout;
    main_center_lay->addLayout(network_lay);
    main_center_lay->addLayout(role_lay);
    main_center_lay->addLayout(account_lay);
    main_center_lay->addLayout(password_lay);
    main_center_lay->addLayout(confirm_lay);
    main_center_lay->addLayout(buttons_lay);

    lay = new QGridLayout;
    lay->addLayout(main_center_lay, 1, 1);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(2, 1);
    lay->setRowStretch(2, 1);
    lay->setRowStretch(0, 1);

    setLayout(lay);
}

RegisterScene::~RegisterScene()
{
    qDebug() << "register scene deleted";
}

void RegisterScene::set_confirm_button_event(const std::function<void ()> &ev)
{
    connect(confirm_button, &QPushButton::clicked, ev);
}

void RegisterScene::set_return_button_event(const std::function<void ()> &ev)
{
    connect(return_button, &QPushButton::clicked, ev);
}

void RegisterScene::set_cancel_button_event(const std::function<void ()> &ev)
{
    connect(cancel_button, &QPushButton::clicked, ev);
}

void RegisterScene::set_role_button_event(const std::function<void ()> &ev)
{
    connect(role_button, &QPushButton::clicked, ev);
}


