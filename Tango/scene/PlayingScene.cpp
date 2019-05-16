
#include <QDebug>
#include <QPushButton>

#include "PlayingScene.h"
#include "../types/TimerWidget.h"
#include "../mainwindow.h"

PlayingScene::PlayingScene(QWidget *parent): Scene (parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);

    auto start_button = new QPushButton(this);
    start_button->setText("开始");
    connect(start_button, &QPushButton::clicked, [this]() mutable {
        this->parent->timer->set_timer(2000);
    });


}

PlayingScene::~PlayingScene()
{
    qDebug() << "playing scene deleted";
}
