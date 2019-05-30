
#include "MainScene.h"
#include "RegisterScene.h"
#include "SelectingScene.h"
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
    network_edit->setText("127.0.0.1");
    port_edit = new QLineEdit;
    port_edit->setText("8888");
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
    password_edit->setEchoMode(QLineEdit::Password);
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

    set_button_events();
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

void MainScene::set_button_events()
{
    /* 登录事件 */
    this->set_sign_in_button_event([this]() mutable {

        QString account_text = this->account_edit->text();
        QString password_text = this->password_edit->text();

        qDebug() << "clicked confirm button" << account_text;
        qDebug() << "clicked confirm button" << password_text;


        if (account_text == "") {
            MessageBox::critical(this, tr("错误"), tr("用户名不能为空"));
            return;
        }

        if (password_text == "") {
            MessageBox::critical(this, tr("错误"), tr("密码不能为空"));
            return;
        }

        /* 根据 remote button的值判断是否远程连接 */
        if (this->remote_button->isChecked()) {
            qDebug() << "checked";

            QHostAddress host_address(this->network_edit->text());
            quint16 server_port = quint16(this->port_edit->text().toShort());

            if (!this->parent->client->setup_remote_connection(host_address, server_port)) {
                MessageBox::critical(this, tr("远程连接失败"), this->parent->client->last_error());
                return;
            }
        } else {
            qDebug() << "not checked";

            if (!this->parent->client->setup_local_connection()) {
                MessageBox::critical(this, tr("本地连接失败"), this->parent->client->last_error());
                return;
            }
        }

        if (this->user_selecting_status == UserStatus::Author) {
            if (!this->parent->client->author_sign_in(account_text, password_text)) {
                MessageBox::critical(this, tr("错误"), "登录失败：" + this->parent->client->last_error());
                return ;
            }
        } else {
            if (!this->parent->client->consumer_sign_in(account_text, password_text)) {
                MessageBox::critical(this, tr("错误"), "登录失败：" + this->parent->client->last_error());
                return ;
            }
        }

        this->parent->selecting_scene->set_visble_buttons();
        this->parent->switch_scene(this->parent->selecting_scene);
    });

    /* 角色变换 */
    this->set_role_button_event([this]() mutable {
        if (this->user_selecting_status == UserStatus::Author) {
            this->role_button->setText("consumer!");
            this->user_selecting_status = UserStatus::Consumer;
        } else {
            this->role_button->setText("author!");
            this->user_selecting_status = UserStatus::Author;
        }
    });

    /* 退出事件 */
    this->set_cancel_button_event([this]() mutable {
        qDebug() << "clicked cancel button";
        this->parent->close();
    });

    /* 转入注册页事件 */
    this->set_sign_up_button_event([this]() mutable {
         this->parent->switch_scene(this->parent->register_scene);
    });
}
