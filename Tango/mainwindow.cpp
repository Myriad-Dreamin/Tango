
#include "mainwindow.h"

/* 工具库 */
#include <QDebug>
#include <QString>
#include <QAction>

#include <QSqlError>
#include <QHostAddress>

/* 控件库 */
#include <QMenu>
#include <QLayout>
#include <QMenuBar>
#include <QStatusBar>

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

#include <QPushButton>
#include <QRadioButton>

/* 场景 */
#include "scene/MainScene.h"
#include "scene/PlayingScene.h"
#include "scene/RegisterScene.h"
#include "scene/CreationScene.h"
#include "scene/SelectingScene.h"

/* 自定类型 */
#include "types/TangoPair.h"
#include "types/UserStatus.h"
#include "types/TimerWidget.h"

/* 客户端代理 */
#include "client/Client.h"


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    this->init_client();

    this->init_menubar();
    this->init_statusbar();

    this->cur_scene = nullptr;
    this->init_main_scene();
    this->init_playing_scene();
    this->init_register_scene();
    this->init_creation_scene();
    this->init_selecting_scene();

    this->setMinimumSize(800, 600);
    this->setWindowTitle("Tango!");

    this->switch_scene(main_scene);
}


MainWindow::~MainWindow()
{
    qDebug() << "mainwindow delete";
    this->client->deleteLater();
    this->main_scene->deleteLater();
    this->playing_scene->deleteLater();
    this->register_scene->deleteLater();
    this->creation_scene->deleteLater();
    this->selecting_scene->deleteLater();
}

/*********************************** Initialize ***********************************/

inline bool MainWindow::init_client()
{
    this->client = new Client(this);
    return true;
}

inline bool init_menubar()
{
    auto main_menubar = menuBar();
    auto menu_item = main_menubar->addMenu("File(&F)");

    menu_item->addAction("&Open", this, SLOT(close()), QKeySequence::Open);
    menu_item->setGeometry(0, 0, this->width(), 30);
    // main_menubar->addMenu(menu_item);

    return true;
}

inline bool init_statusbar()
{
    auto main_statusbar = statusBar();
    this->timer = new TimerWidget(this);

    main_statusbar->addPermanentWidget(new QLabel("tango!"));
    main_statusbar->addPermanentWidget(this->timer);

    return true;
}


inline bool MainWindow::init_playing_scene()
{
    playing_scene = new PlayingScene(this);

    return true;
}


inline bool MainWindow::init_main_scene()
{
    main_scene = new MainScene(this);

    /* 登录事件 */
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

        /* 根据 remote button的值判断是否远程连接 */
        if (this->main_scene->remote_button->isChecked()) {
            qDebug() << "checked";

            QHostAddress host_address(this->main_scene->network_edit->text());
            quint16 server_port = quint16(this->main_scene->port_edit->text().toShort());

            if (!this->client->setup_remote_connection(host_address, server_port)) {
                QMessageBox::critical(this, tr("错误"), this->client->last_error(), QMessageBox::Ok);
                return;
            }
        } else {
            qDebug() << "not checked";

            if (!this->client->setup_local_connection()) {
                QMessageBox::critical(this, tr("错误"), this->client->last_error(), QMessageBox::Ok);
                return;
            }
        }

        bool sign_in_success = false;

        if (this->main_scene->user_selecting_status == UserStatus::Author) {
            sign_in_success = this->author_sign_in(account_text, password_text);
        } else {
            sign_in_success = this->consumer_sign_in(account_text, password_text);
        }

        if (sign_in_success) {
            this->switch_scene(this->selecting_scene);
        }
    });

    /* 角色变换 */
    main_scene->set_role_button_event([this]() mutable {
        if (this->main_scene->user_selecting_status == UserStatus::Author) {
            this->main_scene->role_button->setText("consumer!");
            this->main_scene->user_selecting_status = UserStatus::Consumer;
        } else {
            this->main_scene->role_button->setText("author!");
            this->main_scene->user_selecting_status = UserStatus::Author;
        }
    });

    /* 退出事件 */
    main_scene->set_cancel_button_event([this]() mutable {
        qDebug() << "clicked cancel button";
        this->close();
    });

    /* 转入注册页事件 */
    main_scene->set_sign_up_button_event([this]() mutable {
         this->switch_scene(this->register_scene);
    });

    return true;
}


inline bool MainWindow::init_register_scene()
{
    register_scene = new RegisterScene(this);

    /* 注册事件 */
    register_scene->set_confirm_button_event([this]() mutable {

        QString account_text = this->register_scene->account_edit->text();
        QString password_text = this->register_scene->password_edit->text();
        QString confirm_password_text = this->register_scene->confirm_edit->text();

        qDebug() << "clicked confirm button" << account_text;
        qDebug() << "clicked confirm button" << password_text;
        qDebug() << "clicked confirm button" << confirm_password_text;

        if (account_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("用户名不能为空"), QMessageBox::Ok);
            return;
        }

        if (password_text == "") {
            QMessageBox::critical(this, tr("错误"), tr("密码不能为空"), QMessageBox::Ok);
            return;
        }

        if (password_text != confirm_password_text) {
            qDebug() << "not equal";
            QMessageBox::critical(this, tr("错误"), tr("两次输入密码不一致"), QMessageBox::Ok);
            return;
        }

        /* 根据 remote button的值判断是否远程连接 */
        if (this->register_scene->remote_button->isChecked()) {
            qDebug() << "checked";

            QHostAddress host_address(this->register_scene->network_edit->text());
            quint16 server_port = quint16(this->register_scene->port_edit->text().toShort());

            if (!this->client->setup_remote_connection(host_address, server_port)) {
                QMessageBox::critical(this, tr("错误"), this->client->last_error(), QMessageBox::Ok);
                return;
            }
        } else {
            qDebug() << "not checked";

            if (!this->client->setup_local_connection()) {
                QMessageBox::critical(this, tr("错误"), this->client->last_error(), QMessageBox::Ok);
                return;
            }
        }

        bool sign_up_success = false;

        if (this->register_scene->user_selecting_status == UserStatus::Author) {
            sign_up_success = this->author_sign_up(account_text, password_text);
        } else {
            sign_up_success = this->consumer_sign_up(account_text, password_text);
        }

        if (sign_up_success) {
            this->switch_scene(this->selecting_scene);
        }
    });

    /* 角色变换 */
    register_scene->set_role_button_event([this]() mutable {
        if (this->register_scene->user_selecting_status == UserStatus::Author) {
            this->register_scene->role_button->setText("consumer!");
            this->register_scene->user_selecting_status = UserStatus::Consumer;
        } else {
            this->register_scene->role_button->setText("author!");
            this->register_scene->user_selecting_status = UserStatus::Author;
        }
    });

    /* 退出事件 */
    register_scene->set_cancel_button_event([this]() mutable {
        qDebug() << "clicked cancel button";
        this->close();
    });

    /* 返回登录界面事件 */
    register_scene->set_return_button_event([this]() mutable {
        qDebug() << "clicked return button" << this->main_scene;
        this->switch_scene(this->main_scene);
    });

    return true;
}

inline bool MainWindow::init_selecting_scene()
{
    this->selecting_scene = new SelectingScene(this);

    /* 前往创造界面事件 */
    this->selecting_scene->set_creation_button_event([this]() mutable {
        qDebug() << "clicked creation button";

        this->switch_scene(this->creation_scene);
    });

    /* 前往游玩界面事件 */
    this->selecting_scene->set_play_button_event([this]() mutable {
        qDebug() << "clicked play button";

        this->switch_scene(this->playing_scene);
    });

    return true;
}

inline bool MainWindow::init_creation_scene()
{
    this->creation_scene = new CreationScene(this);

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


/************************************* Player *************************************/

bool MainWindow::author_sign_in(QString account, QString password)
{
    if (!this->client->author_sign_in(account, password)) {
        QMessageBox::critical(this, tr("错误"), "登录失败：" + this->client->last_error(), QMessageBox::Ok);
        return false;
    }

    return true;
}

bool MainWindow::author_sign_up(QString account, QString password)
{
    if (!this->client->author_sign_up(account, password)) {
        QMessageBox::critical(this, tr("错误"), "注册失败：" + this->client->last_error(), QMessageBox::Ok);
        return false;
    }

    return true;
}

bool MainWindow::consumer_sign_in(QString account, QString password)
{
    if (!this->client->consumer_sign_in(account, password)) {
        QMessageBox::critical(this, tr("错误"), "登录失败：" + this->client->last_error(), QMessageBox::Ok);
        return false;
    }

    return true;
}

bool MainWindow::consumer_sign_up(QString account, QString password)
{
    if (!this->client->consumer_sign_up(account, password)) {
        QMessageBox::critical(this, tr("错误"), "注册失败：" + this->client->last_error(), QMessageBox::Ok);
        return false;
    }

    return true;
}

bool MainWindow::submit_creation_table(const std::vector<TangoPair> &tango_pairs)
{
    if (!this->client->submit_tango_items(tango_pairs)) {
        QMessageBox::critical(this, tr("错误"), "添加单词失败：" + this->client->last_error(), QMessageBox::Ok);
        return false;
    }

    return true;
}
