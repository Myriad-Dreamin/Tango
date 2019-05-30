
#include "RegisterScene.h"
#include "SelectingScene.h"
#include "MainScene.h"
#include "../mainwindow.h"


RegisterScene::RegisterScene(QWidget *parent): Scene(parent)
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

    auto confirm_lay = new QHBoxLayout;
    auto confirm_lab = new QLabel("确认: ");
    confirm_edit = new QLineEdit;
    confirm_edit->setEchoMode(QLineEdit::Password);
    confirm_lay->addWidget(confirm_lab);
    confirm_lay->addWidget(confirm_edit, 1);

    auto buttons_lay = new QHBoxLayout;

    buttons_lay->addStretch(1);

    confirm_button = new QPushButton;
    confirm_button->setText("确认");
    confirm_button->setMinimumHeight(32);
    buttons_lay->addWidget(confirm_button, 3);

    buttons_lay->addStretch(1);

    cancel_button = new QPushButton;
    cancel_button->setText("取消");
    cancel_button->setMinimumHeight(32);
    buttons_lay->addWidget(cancel_button, 3);

    buttons_lay->addStretch(1);

    return_button = new QPushButton;
    return_button->setText("返回");
    return_button->setMinimumHeight(32);
    buttons_lay->addWidget(return_button, 3);

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
    main_center_lay->addLayout(confirm_lay, 10);
    main_center_lay->addStretch(1);
    main_center_lay->addLayout(buttons_lay, 10);
    main_center_lay->addStretch(20);

    lay = new QHBoxLayout(this);
    lay->addStretch(2);
    lay->addLayout(main_center_lay, 7);
    lay->addStretch(2);

    this->setLayout(lay);

    set_button_events();
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

void RegisterScene::set_button_events()
{
    /* 注册事件 */
    this->set_confirm_button_event([this]() mutable {

        QString account_text = this->account_edit->text();
        QString password_text = this->password_edit->text();
        QString confirm_password_text = this->confirm_edit->text();

        qDebug() << "clicked confirm button" << account_text;
        qDebug() << "clicked confirm button" << password_text;
        qDebug() << "clicked confirm button" << confirm_password_text;

        if (account_text == "") {
            MessageBox::critical(this, tr("错误"), tr("用户名不能为空"));
            return;
        }

        if (password_text == "") {
            MessageBox::critical(this, tr("错误"), tr("密码不能为空"));
            return;
        }

        if (password_text != confirm_password_text) {
            qDebug() << "not equal";
            MessageBox::critical(this, tr("错误"), tr("两次输入密码不一致"));
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

        bool sign_up_success = false;

        if (this->user_selecting_status == UserStatus::Author) {
            sign_up_success = this->parent->author_sign_up(account_text, password_text);
        } else {
            sign_up_success = this->parent->consumer_sign_up(account_text, password_text);
        }

        if (sign_up_success) {
            if (!this->remote_button->isChecked()) {
                int query_count;
                if (!this->parent->client->query_users(query_count)) {
                    MessageBox::critical(this, tr("查询用户总量失败"), this->parent->client->last_error());
                    return;
                }
                qDebug() << "querying" << query_count;
                if (query_count > 0) {
                    if (!this->parent->client->init_default_tangos()) {
                        MessageBox::critical(this, tr("初始化词库失败"), this->parent->client->last_error());
                        return;
                    }
                }
            }

            this->parent->selecting_scene->set_visble_buttons();
            this->parent->switch_scene(this->parent->selecting_scene);
        }
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

    /* 返回登录界面事件 */
    this->set_return_button_event([this]() mutable {
        qDebug() << "clicked return button" << this->parent->main_scene;
        this->parent->client->logout();
        this->parent->switch_scene(this->parent->main_scene);
    });
}
