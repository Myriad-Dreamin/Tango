
#include <functional>

#include <QPushButton>
#include <QLayout>
#include <QDebug>

#include "SelectingScene.h"
#include "../mainwindow.h"

SelectingScene::SelectingScene(QWidget *parent): Scene(parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);

    auto main_center_lay = new QVBoxLayout;

    creation_button = new QPushButton;
    creation_button->setText("开始创造");
    main_center_lay->addWidget(creation_button);
    play_button = new QPushButton;
    play_button->setText("开始游玩");
    main_center_lay->addWidget(play_button);
    multi_play_button = new QPushButton;
    multi_play_button->setText("多人对战");
    main_center_lay->addWidget(multi_play_button);
    player_list_button = new QPushButton;
    player_list_button->setText("其他玩家");
    main_center_lay->addWidget(player_list_button);
    ranking_authors_button = new QPushButton;
    ranking_authors_button->setText("作者排行榜");
    main_center_lay->addWidget(ranking_authors_button);
    ranking_consumers_button = new QPushButton;
    ranking_consumers_button->setText("读者排行榜");
    main_center_lay->addWidget(ranking_consumers_button);



    lay = new QGridLayout;
    lay->addLayout(main_center_lay, 1, 1, 3, 3);
    lay->setRowStretch(1, 3);
    lay->setColumnStretch(1, 3);
    lay->setColumnStretch(0, 1);
    lay->setColumnStretch(5, 1);
    lay->setRowStretch(0, 1);
    lay->setRowStretch(5, 1);
    setLayout(lay);
}

SelectingScene::~SelectingScene()
{
    qDebug() << "selecting scene deleted";
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
