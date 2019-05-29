
#include "mainwindow.h"

# define QT_NO_DEBUG_OUTPUT

/* 工具库 */
#include <QDebug>
#include <QString>
#include <QAction>
#include <QFile>


#include <QSqlError>
#include <QHostAddress>

/* 控件库 */
#include <QMenu>
#include <QLayout>
#include <QMenuBar>
#include <QStatusBar>

#include <QLabel>
#include <QLineEdit>

#include <QPushButton>
#include <QRadioButton>

/* 场景 */
#include "scene/MainScene.h"
#include "scene/PlayingScene.h"
#include "scene/MultiPlayingScene.h"
#include "scene/PlaySubScene.h"
#include "scene/RegisterScene.h"
#include "scene/CreationScene.h"
#include "scene/SelectingScene.h"
#include "scene/PlaySettleScene.h"
#include "scene/RankingAuthorsScene.h"
#include "scene/RankingConsumersScene.h"
#include "scene/QueryUsersScene.h"

/* 自定类型 */
#include "../TangoCommon/types/TangoPair.h"
#include "../TangoCommon/types/UserStatus.h"
#include "../TangoCommon/types/TimerWidget.h"
#include "../TangoCommon/types/Logger.h"
#include "../TangoCommon/types/MessageBox.h"

/* 客户端代理 */
#include "../TangoCommon/client/Client.h"


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{

    this->client = nullptr;
    this->cur_scene = nullptr;
    this->main_scene = nullptr;
    this->playing_scene = nullptr;
    this->multiplaying_scene = nullptr;
    this->playsub_scene = nullptr;
    this->playset_scene = nullptr;
    this->register_scene = nullptr;
    this->creation_scene = nullptr;
    this->selecting_scene = nullptr;
    this->ranking_authors_scene = nullptr;
    this->ranking_consumers_scene = nullptr;
    this->query_users_scene = nullptr;


    this->init_main_scene();
    this->init_playing_scene();
    this->init_multiplaying_scene();
    this->init_playsub_scene();
    this->init_register_scene();
    this->init_creation_scene();
    this->init_selecting_scene();
    this->init_playset_scene();
    this->init_ranking_authors_scene();
    this->init_ranking_consumers_scene();
    this->init_query_users_scene();

    this->init_client();
    this->init_menubar();
    this->init_statusbar();

    this->switch_scene(main_scene);

    this->setMinimumSize(800, 600);
    this->setWindowTitle("Tango!");

    QFile qssf(":/qss/main.qss");
    qssf.open(QFile::ReadOnly);
    QString reading = qssf.readAll();
    // qDebug() << reading;
    this->setStyleSheet(reading);
    qssf.close();
}


MainWindow::~MainWindow()
{
    qDebug() << "mainwindow delete";
    this->client->logout();
    this->client->deleteLater();
    this->main_scene->deleteLater();
    this->playing_scene->deleteLater();
    this->register_scene->deleteLater();
    this->creation_scene->deleteLater();
    this->selecting_scene->deleteLater();
    this->ranking_authors_scene->deleteLater();
    this->ranking_consumers_scene->deleteLater();
    this->query_users_scene->deleteLater();
}

/*********************************** Initialize ***********************************/

inline bool MainWindow::init_client()
{
    this->client = new Client(this);
    return true;
}

inline bool MainWindow::init_menubar()
{
    auto main_menubar = menuBar();
    auto menu_item = main_menubar->addMenu("File(&F)");

    menu_item->addAction("&Open", this, SLOT(close()), QKeySequence::Open);
    menu_item->setGeometry(0, 0, this->width(), 30);
    main_menubar->addMenu(menu_item);

    return true;
}

inline bool MainWindow::init_statusbar()
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

    this->playing_scene->hide();
    return true;
}

bool MainWindow::init_multiplaying_scene()
{
    multiplaying_scene = new MultiPlayingScene(this);

    this->multiplaying_scene->hide();
    return true;
}


inline bool MainWindow::init_main_scene()
{

    main_scene = new MainScene(this);


    this->main_scene->hide();
    return true;
}


inline bool MainWindow::init_register_scene()
{
    register_scene = new RegisterScene(this);


    this->register_scene->hide();
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

    this->selecting_scene->set_multi_play_button_event([this]() mutable {
        qDebug() << "clicked play button";

        this->switch_scene(this->multiplaying_scene);
    });

    this->selecting_scene->set_ranking_authors_button_event([this]() mutable {
        qDebug() << "clicked ranking button";

        this->ranking_authors_scene->switch_page(1);
        this->switch_scene(this->ranking_authors_scene);
    });

    this->selecting_scene->set_ranking_consumers_button_event([this]() mutable {
        qDebug() << "clicked ranking button";

        this->ranking_consumers_scene->switch_page(1);
        this->switch_scene(this->ranking_consumers_scene);
    });

    this->selecting_scene->set_player_list_button_event([this]() mutable {
        qDebug() << "clicked player list button";

        this->switch_scene(this->query_users_scene);
    });

    this->selecting_scene->set_return_button_event([this]() mutable {
        qDebug() << "clicked return button";

        this->client->logout();
        this->switch_scene(this->main_scene);
    });

    this->selecting_scene->hide();
    return true;
}

bool MainWindow::init_playset_scene()
{
    this->playset_scene = new PlaySettleScene(this);

    this->playset_scene->hide();
    return true;
}

bool MainWindow::init_ranking_authors_scene()
{
    this->ranking_authors_scene = new RankingAuthorsScene(this);

    this->ranking_authors_scene->hide();
    return true;
}

bool MainWindow::init_ranking_consumers_scene()
{
    this->ranking_consumers_scene = new RankingConsumersScene(this);

    this->ranking_consumers_scene->hide();
    return true;
}

bool MainWindow::init_query_users_scene()
{
    this->query_users_scene = new QueryUsersScene(this);

    this->query_users_scene->hide();
    return true;
}

inline bool MainWindow::init_creation_scene()
{
    this->creation_scene = new CreationScene(this);

    this->creation_scene->hide();
    return true;
}

inline bool MainWindow::init_playsub_scene()
{
    this->playsub_scene = new PlaySubScene(this);

    this->playsub_scene->hide();
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
        MessageBox::critical(this, tr("错误"), "登录失败：" + this->client->last_error());
        return false;
    }

    return true;
}

bool MainWindow::author_sign_up(QString account, QString password)
{
    if (!this->client->author_sign_up(account, password)) {
        MessageBox::critical(this, tr("错误"), "注册失败：" + this->client->last_error());
        return false;
    }

    return true;
}

bool MainWindow::consumer_sign_in(QString account, QString password)
{
    if (!this->client->consumer_sign_in(account, password)) {
        MessageBox::critical(this, tr("错误"), "登录失败：" + this->client->last_error());
        return false;
    }

    return true;
}

bool MainWindow::consumer_sign_up(QString account, QString password)
{
    if (!this->client->consumer_sign_up(account, password)) {
        MessageBox::critical(this, tr("错误"), "注册失败：" + this->client->last_error());
        return false;
    }

    return true;
}

bool MainWindow::submit_creation_table(const std::vector<TangoPair> &tango_pairs)
{
    if (!this->client->submit_tango_items(tango_pairs)) {
        MessageBox::critical(this, tr("错误"), "添加单词失败：" + this->client->last_error());
        return false;
    }

    return true;
}
