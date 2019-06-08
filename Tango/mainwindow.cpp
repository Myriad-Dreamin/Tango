
#include "mainwindow.h"

# define QT_NO_DEBUG_OUTPUT

/* 工具库 */
#include <QDebug>
#include <QString>
#include <QAction>
#include <QFile>
#include <QSettings>

#include <QSqlError>
#include <QHostAddress>

/* 控件库 */
#include <QMenu>
#include <QLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>

#include <QLabel>
#include <QLineEdit>

#include <QPushButton>
#include <QRadioButton>

/* 场景 */
#include "scene/MainScene.h"
#include "scene/LoginScene.h"
#include "scene/ConfigScene.h"
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

#include "../TangoCommon/component/TimerWidget.h"
#include "../TangoCommon/component/Logger.h"
#include "../TangoCommon/component/MessageBox.h"
#include "../TangoCommon/component/ConfigSet.h"


/* 客户端代理 */
#include "../TangoCommon/client/Client.h"


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent)
{
    this->logger = Logger::get_logger("main");
    this->qconfig = nullptr;
    this->load_configs();

    this->client = nullptr;
    this->cur_scene = nullptr;
    this->login_scene = nullptr;
    this->config_scene = nullptr;
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
    this->init_login_scene();
    this->init_config_scene();
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

    this->reset_screen_size();

    this->setWindowTitle("Tango!");

    QFile qssf(":/qss/main.qss");
    qssf.open(QFile::ReadOnly);
    QString reading = qssf.readAll();
    this->setStyleSheet(reading);
    qssf.close();

}


MainWindow::~MainWindow()
{
    logger->info() << "mainwindow delete";
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

    this->client->disconnected_callback = [this](){
        qDebug() << "disconnected";
        MessageBox::critical(this, "失去连接", "与服务器失去连接，未设置重连选项，故退出登录");
        this->switch_scene(this->main_scene);
    };

    return true;
}

inline bool MainWindow::init_menubar()
{
    auto main_menubar = menuBar();
    auto menu_file_item = main_menubar->addMenu("菜单(&I)");

    auto return_action = menu_file_item->addAction("返回主界面");
    return_action->setShortcut(Qt::CTRL | Qt::Key_Home);
    connect(return_action, &QAction::triggered, [this](){
        if (this->cur_scene != this->playing_scene && this->cur_scene != this->playsub_scene) {
            this->client->logout();
            this->switch_scene(this->main_scene);
        }
    });

    auto load_action = menu_file_item->addAction("加载配置");
    load_action->setShortcut(Qt::CTRL | Qt::Key_L);
    connect(load_action, &QAction::triggered, [this](){
        auto file_name = QFileDialog::getOpenFileName(this, tr("打开配置文件"), "", tr("Config Files (*.ini);;Config Files (*.txt)"));
        this->load_configs(file_name);
        if (this->cur_scene == this->config_scene) {
            this->config_scene->refill();
        }
    });

    auto full_screen_action = menu_file_item->addAction("");
    if (this->isFullScreen()) {
        full_screen_action->setText("取消全屏");
    } else {
        full_screen_action->setText("全屏");
    }
    full_screen_action->setShortcut(Qt::Key_F11);


    connect(full_screen_action, &QAction::triggered, [this](){
        if (this->isFullScreen()) {
            this->qconfig->setValue("display/fullscreen_mode", "no");
        } else {
            this->qconfig->setValue("display/fullscreen_mode", "yes");
        }
        this->reset_screen_size();
    });
    connect(this, &MainWindow::screen_size_changed, [this, full_screen_action]() {
        if (this->isFullScreen()) {
            full_screen_action->setText("取消全屏");
        } else {
            full_screen_action->setText("全屏");
        }
    });


    auto close_action = menu_file_item->addAction("关闭");
    close_action->setShortcut(Qt::ALT | Qt::Key_F4);
    connect(close_action, &QAction::triggered, [this](){
        this->close();
    });

    menu_file_item->setGeometry(0, 0, this->width(), 30);
    main_menubar->addMenu(menu_file_item);

    auto menu_config_item = main_menubar->addMenu("设定(&C)");

    menu_config_item->addAction("设置");

    menu_config_item->setGeometry(0, 0, this->width(), 30);
    main_menubar->addMenu(menu_config_item);




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


inline bool MainWindow::init_login_scene()
{

    login_scene = new LoginScene(this);


    this->login_scene->hide();
    return true;
}


inline bool MainWindow::init_config_scene()
{

    config_scene = new ConfigScene(this);


    this->config_scene->hide();
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

void MainWindow::switch_scene(Scene *to_set)
{
    logger->info() << this->cur_scene << to_set;

    if (this->cur_scene) {
        this->cur_scene->on_exiting();
        this->cur_scene->hide();
    }

    this->takeCentralWidget();
    this->setCentralWidget(to_set);
    this->cur_scene = to_set;
    this->cur_scene->on_incoming();
    this->cur_scene->show();

    return;
}

/************************************* Config *************************************/


bool MainWindow::load_configs(const char* file_path)
{
    if (this->qconfig) {
        this->qconfig->deleteLater();
    }
    this->qconfig = new ConfigSet(file_path, QSettings::IniFormat, this);
//    if (!this->qconfig) {
//        MessageBox::critical(this, "错误", "未能读取config.ini文件，将使用默认设定");

//        return false;
//    }
    this->set_default_configs();
    this->qconfig->sync();
    return true;
}

bool MainWindow::load_configs(QString file_path)
{
    if (this->qconfig) {
        this->qconfig->deleteLater();
    }
    this->qconfig = new ConfigSet(file_path, QSettings::IniFormat, this);

    this->set_default_configs();
    this->qconfig->sync();
    return true;
}


bool MainWindow::set_default_configs()
{
    this->qconfig->set_default_value("limit/default_creation_table_items_count", 3);
    this->qconfig->set_default_value("display/style", "800x600");
    this->qconfig->set_default_value("display/fullscreen_mode", "yes");
    this->qconfig->set_default_value("mysql/host", "localhost");
    this->qconfig->set_default_value("mysql/basename", "tango");
    this->qconfig->set_default_value("mysql/user", "tangosql");
    this->qconfig->set_default_value("mysql/password", "123456");
    return true;
}

void MainWindow::reset_screen_size()
{
    qDebug() << "reset_screen_size";
    auto dis_style = this->qconfig->at("display/style").toString();
    auto line_list = dis_style.split("x");
    this->setFixedSize(line_list[0].toInt(), line_list[1].toInt());
    if (this->qconfig->at("display/fullscreen_mode").toString() == "yes") {
        this->showFullScreen();
    } else {
        this->showNormal();
    }
    emit this->screen_size_changed();
}

