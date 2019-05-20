
#include "PlayingScene.h"

#include <algorithm>

#include <QDebug>

#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>

#include "PlaySubScene.h"
#include "PlaySettleScene.h"
#include "../mainwindow.h"
#include "../client/Client.h"
#include "../client/GameAutomation.h"
#include "../client/GameConfig.h"

#include "../types/TimerWidget.h"
#include "../types/RetriveMode.h"


GameAutomation *PlayingScene::default_automate()
{
    return this->parent->client->start_game_event(game_config, 5, RetriveMode::Hard);
}

GameAutomation *PlayingScene::more_complex_automate()
{
    return this->parent->client->start_game_event(
        &DECREASE_TIMECONFIG,
        std::max(1, rand() % ((this->parent->client->consumer_level() + 1) << 1)),
        RetriveMode::Hard
    );
}


std::function<void()> PlayingScene::single_round()
{
    return [this]() mutable {

        auto automate = this->more_complex_automate();

        if (automate == nullptr) {
            QMessageBox::critical(this, "错误", this->parent->client->last_error(), QMessageBox::Ok);
            return;
        }

        automate->setParent(this);

        connect(automate, &GameAutomation::start_game, [this]() mutable {
            qDebug() << "start game";
            this->parent->switch_scene(this->parent->playsub_scene);
        });
        connect(automate, &GameAutomation::new_tango, [this](TangoPair tango) mutable {
            qDebug() << "retreving" << tango;
            this->parent->playsub_scene->key_label->setText(tango.first);
            this->parent->playsub_scene->value_label->setText(tango.second);
            this->parent->playsub_scene->key_label->show();
            this->parent->playsub_scene->value_label->show();


            this->parent->playsub_scene->user_ret->hide();
            this->parent->playsub_scene->answer_button->hide();

            this->parent->timer->set_timer(5000);
        });
        connect(automate, &GameAutomation::tango_faded, [this]() mutable {
            qDebug() << "tango faded";
            this->parent->playsub_scene->key_label->hide();
            this->parent->playsub_scene->value_label->hide();

            this->parent->playsub_scene->user_ret->show();
            this->parent->playsub_scene->answer_button->show();

            this->parent->timer->set_timer(10000);
        });
        connect(automate, &GameAutomation::answer_failed, []() mutable {
            qDebug() << "answer failed";
        });
        connect(automate, &GameAutomation::success, [this, automate]() mutable {
            qDebug() << "success";
            this->settle_game(automate);
        });
        connect(automate, &GameAutomation::failed, [this, automate]() mutable {
            qDebug() << "failed";
            this->settle_game(automate);
        });
        connect(this->parent->playsub_scene->answer_button, &QPushButton::clicked, [this, automate]() mutable {

            if (this->parent->playsub_scene->user_ret->text() == this->parent->playsub_scene->key_label->text()) {
                automate->make_answer_success_slotter()();
                this->parent->playsub_scene->user_ret->setText("");
            }

        });
        connect(this->parent->playsub_scene->stop_button, &QPushButton::clicked, [this, automate]() mutable {
            this->settle_game(automate);
        });

        automate->start();
    };
}


PlayingScene::PlayingScene(QWidget *parent): Scene (parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);
    game_config = new GameConfig();

    start_button = new QPushButton;
    start_button->setText("开始");

    center_lay = new QVBoxLayout;
    center_lay->addWidget(start_button);

    connect(start_button, &QPushButton::clicked, this->single_round());

    this->lay = new QGridLayout;
    this->lay->addLayout(center_lay, 1, 1, 3, 3);
    this->lay->setColumnStretch(0, 1);
    this->lay->setColumnStretch(4, 1);
    this->lay->setRowStretch(4, 1);
    this->lay->setRowStretch(0, 1);

    this->setLayout(this->lay);
}

PlayingScene::~PlayingScene()
{
    qDebug() << "playing scene deleted";
    delete game_config;
}

void PlayingScene::settle_game(GameAutomation *automate)
{
    qDebug() << "setting...";
    disconnect(this->parent->playsub_scene->answer_button, nullptr, nullptr, nullptr);
    disconnect(this->parent->playsub_scene->stop_button, nullptr, nullptr, nullptr);
    this->parent->timer->stop_timer();

    int origin_level = this->parent->client->consumer_level();
    this->parent->playset_scene->from_exp->setNum(this->parent->client->consumer_exp());
    this->parent->playset_scene->from_level->setNum(this->parent->client->consumer_level());

    this->parent->client->settle_game_event(automate);
    this->parent->playset_scene->to_exp->setNum(this->parent->client->consumer_exp());
    this->parent->playset_scene->to_level->setNum(this->parent->client->consumer_level());

    if (this->parent->client->consumer_level() - origin_level > 0) {
        this->parent->playset_scene->level_flag->setText("↑");
    } else {
        this->parent->playset_scene->level_flag->setText("-");
    }
    qDebug() << "setting...";

    automate->deleteLater();
    this->parent->switch_scene(this->parent->playset_scene);
    this->parent->playsub_scene->user_ret->setText("");
}
