
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include "QDebug"
#include "MainScene.h"


MainScene::MainScene(QWidget *parent): Scene(parent)
{
    auto account_lay = new QHBoxLayout;
    auto account_lab = new QLabel("账户: ");
    auto account_edit = new QLineEdit;
    account_lay->addWidget(account_lab);
    account_lay->addWidget(account_edit, 1);

    auto password_lay = new QHBoxLayout;
    auto password_lab = new QLabel("密码: ");
    auto password_edit = new QLineEdit;
    password_lay->addWidget(password_lab);
    password_lay->addWidget(password_edit, 1);

    auto buttons_lay = new QHBoxLayout;

    buttons_lay->addStretch(1);

    auto sign_in_button = new QPushButton;
    sign_in_button->setText("登录");
    buttons_lay->addWidget(sign_in_button);

    auto cancel_button = new QPushButton;
    cancel_button->setText("取消");
    buttons_lay->addWidget(cancel_button);

//    connect(cancel_button, &QPushButton::clicked, [this]() mutable {
//        qDebug() << "clicked cancel button";
//        this->close();
//    });

    auto sign_up_button = new QPushButton;
    sign_up_button->setText("注册");

//    connect(sign_up_button, &QPushButton::clicked, [this]() mutable {
//        this->switch_scene(this->register_scene);
//    });

    buttons_lay->addWidget(sign_up_button);

    buttons_lay->addStretch(1);

    auto main_center_lay = new QVBoxLayout;
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

}
