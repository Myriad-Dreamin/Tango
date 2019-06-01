
#include "MainScene.h"
#include "ConfigScene.h"
#include "LoginScene.h"
#include "RegisterScene.h"
#include "../mainwindow.h"


MainScene::MainScene(MainWindow *parent): Scene(parent)
{
    this->parent = parent;

    auto main_center_lay = new QVBoxLayout;

    login_button = new QPushButton;
    login_button->setText("登录");
    login_button->setMinimumHeight(32);
    main_center_lay->addWidget(login_button);

    register_button = new QPushButton;
    register_button->setText("注册");
    register_button->setMinimumHeight(32);
    main_center_lay->addWidget(register_button);

    setting_button = new QPushButton;
    setting_button->setText("设置");
    setting_button->setMinimumHeight(32);
    main_center_lay->addWidget(setting_button);

    exit_button = new QPushButton;
    exit_button->setText("退出");
    exit_button->setMinimumHeight(32);
    main_center_lay->addWidget(exit_button);

    lay = new QGridLayout;
    lay->addLayout(main_center_lay, 1, 1, 3, 3);
    lay->setRowStretch(1, 3);
    lay->setColumnStretch(1, 3);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(5, 1);
    lay->setRowStretch(0, 1);
    lay->setRowStretch(5, 1);
    setLayout(lay);

    set_button_events();
}

MainScene::~MainScene()
{
    qDebug() << "main scene deleted";
}

void MainScene::set_login_button_event(const std::function<void ()> &ev)
{
    connect(login_button, &QPushButton::clicked, ev);
}

void MainScene::set_register_button_event(const std::function<void ()> &ev)
{
    connect(register_button, &QPushButton::clicked, ev);
}

void MainScene::set_setting_button_event(const std::function<void ()> &ev)
{
    connect(setting_button, &QPushButton::clicked, ev);
}

void MainScene::set_exit_button_event(const std::function<void ()> &ev)
{
    connect(exit_button, &QPushButton::clicked, ev);
}

void MainScene::set_button_events()
{
    /* 前往登录界面事件 */
    this->set_login_button_event([this]() mutable {
        qDebug() << "clicked login button";

        this->parent->switch_scene(this->parent->login_scene);
    });

    /* 前往注册界面事件 */
    this->set_register_button_event([this]() mutable {
        qDebug() << "clicked register button";

        this->parent->switch_scene(this->parent->register_scene);
    });

    /* 前往设置界面事件 */
    this->set_setting_button_event([this]() mutable {
        qDebug() << "clicked setting button";

        this->parent->switch_scene(this->parent->config_scene);
    });

    /* 退出事件 */
    this->set_exit_button_event([this]() mutable {
        qDebug() << "clicked exit button";

        this->parent->close();
    });

}
