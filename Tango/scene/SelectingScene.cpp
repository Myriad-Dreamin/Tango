
#include "SelectingScene.h"
#include "CreationScene.h"
#include "PlayingScene.h"
#include "MultiPlayingScene.h"
#include "QueryUsersScene.h"
#include "RankingAuthorsScene.h"
#include "RankingConsumersScene.h"
#include "MainScene.h"
#include "../mainwindow.h"


SelectingScene::SelectingScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);

    auto main_center_lay = new QVBoxLayout;

    creation_button = new QPushButton;
    creation_button->setText("开始创造");
    creation_button->setMinimumHeight(32);
    main_center_lay->addWidget(creation_button);
    
    play_button = new QPushButton;
    play_button->setText("开始游玩");
    play_button->setMinimumHeight(32);
    main_center_lay->addWidget(play_button);
    
    multi_play_button = new QPushButton;
    multi_play_button->setText("多人对战");
    multi_play_button->setMinimumHeight(32);
    main_center_lay->addWidget(multi_play_button);
    
    player_list_button = new QPushButton;
    player_list_button->setText("其他玩家");
    player_list_button->setMinimumHeight(32);
    main_center_lay->addWidget(player_list_button);
    
    ranking_authors_button = new QPushButton;
    ranking_authors_button->setText("作者排行榜");
    ranking_authors_button->setMinimumHeight(32);
    main_center_lay->addWidget(ranking_authors_button);
    
    ranking_consumers_button = new QPushButton;
    ranking_consumers_button->setText("读者排行榜");
    ranking_consumers_button->setMinimumHeight(32);
    main_center_lay->addWidget(ranking_consumers_button);
    
    return_button = new QPushButton;
    return_button->setText("返回登录/注册界面");
    return_button->setMinimumHeight(32);
    main_center_lay->addWidget(return_button);

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

SelectingScene::~SelectingScene()
{
    qDebug() << "selecting scene deleted";
}

bool SelectingScene::set_visble_buttons()
{
    if (this->parent->client->consumer_logining()) {
        this->play_button->show();
    } else {
        this->play_button->hide();
    }

    if (this->parent->client->is_remote_server_connected()) {
        this->multi_play_button->show();
    } else {
        this->multi_play_button->hide();
    }

    if (this->parent->client->author_logining()) {
        this->creation_button->show();
    } else {
        this->creation_button->hide();
    }
    return true;
}

void SelectingScene::set_creation_button_event(const std::function<void ()> &ev)
{
    connect(creation_button, &QPushButton::clicked, ev);
}


void SelectingScene::set_play_button_event(const std::function<void ()> &ev)
{
    connect(play_button, &QPushButton::clicked, ev);
}


void SelectingScene::set_multi_play_button_event(const std::function<void ()> &ev)
{
    connect(multi_play_button, &QPushButton::clicked, ev);
}


void SelectingScene::set_player_list_button_event(const std::function<void ()> &ev)
{
    connect(player_list_button, &QPushButton::clicked, ev);
}


void SelectingScene::set_ranking_authors_button_event(const std::function<void ()> &ev)
{
    connect(ranking_authors_button, &QPushButton::clicked, ev);
}

void SelectingScene::set_ranking_consumers_button_event(const std::function<void ()> &ev)
{
    connect(ranking_consumers_button, &QPushButton::clicked, ev);
}

void SelectingScene::set_return_button_event(const std::function<void ()> &ev)
{
    connect(return_button, &QPushButton::clicked, ev);
}

void SelectingScene::set_button_events()
{
    /* 前往创造界面事件 */
    this->set_creation_button_event([this]() mutable {
        qDebug() << "clicked creation button";

        this->parent->switch_scene(this->parent->creation_scene);
    });

    /* 前往游玩界面事件 */
    this->set_play_button_event([this]() mutable {
        qDebug() << "clicked play button";

        this->parent->switch_scene(this->parent->playing_scene);
    });

    this->set_multi_play_button_event([this]() mutable {
        qDebug() << "clicked play button";

        this->parent->switch_scene(this->parent->multiplaying_scene);
    });

    this->set_ranking_authors_button_event([this]() mutable {
        qDebug() << "clicked ranking button";

        this->parent->ranking_authors_scene->switch_page(1);
        this->parent->switch_scene(this->parent->ranking_authors_scene);
    });

    this->set_ranking_consumers_button_event([this]() mutable {
        qDebug() << "clicked ranking button";

        this->parent->ranking_consumers_scene->switch_page(1);
        this->parent->switch_scene(this->parent->ranking_consumers_scene);
    });

    this->set_player_list_button_event([this]() mutable {
        qDebug() << "clicked player list button";

        this->parent->switch_scene(this->parent->query_users_scene);
    });

    this->set_return_button_event([this]() mutable {
        qDebug() << "clicked return button";

        qDebug() << this->parent->client->logout() << this->parent->client->last_error();
        this->parent->switch_scene(this->parent->main_scene);
    });

}
