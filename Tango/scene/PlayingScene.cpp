
#include "PlayingScene.h"


std::function<AbstractGameAutomation *()> PlayingScene::default_automate()
{
    if (game_config != nullptr) {
        delete game_config;
    }
    game_config = new GameConfig();
    return [this]() mutable -> AbstractGameAutomation * {
        return this->parent->client->start_game_event(game_config, 5, RetriveMode::DefaultMode);
    };
}

std::function<AbstractGameAutomation *()> PlayingScene::easy_increment_automate()
{
    std::uniform_int_distribution<int> uni_gen(
        0,
        31 - (30 + this->parent->client->consumer_level())/(this->parent->client->consumer_level() + 1)
    );
    auto uni_rand = std::bind(uni_gen, std::mt19937(static_cast<unsigned int>(time(nullptr)) * 12345U + 3U));

    if (game_config != nullptr) {
        delete game_config;
    }
    game_config = new GameConfig(GameConfigMode::HARD_EASYMODE_CONFIG);

    return [this, uni_rand]() mutable -> AbstractGameAutomation * {
        return this->parent->client->start_game_event(
            this->game_config,
            std::max(1, static_cast<int>(uni_rand())) ,
            RetriveMode::EasyMode
        );
    };
}

void normal_increment_automate(int &exp, TangoPair tango, int success_count)
{
    exp = exp + static_cast<int>(tango.first.length() * (1.2 + success_count * 0.02) + 0.99);
}

std::function<AbstractGameAutomation *()> PlayingScene::normal_increment_automate()
{
    std::uniform_int_distribution<int> uni_gen(
        31 - (30 + this->parent->client->consumer_level())/(this->parent->client->consumer_level() + 1),
        51 - (50 + this->parent->client->consumer_level())/(this->parent->client->consumer_level() + 1)
    );
    auto uni_rand = std::bind(uni_gen, std::mt19937(static_cast<unsigned int>(time(nullptr)) * 23345U + 3U));

    if (game_config != nullptr) {
        delete game_config;
    }
    game_config = new GameConfig(GameConfigMode::HARD_NORMALMODE_CONFIG);

    return [this, uni_rand]() mutable -> AbstractGameAutomation * {
        return this->parent->client->start_game_event(
            this->game_config,
            std::max(1, static_cast<int>(uni_rand())) ,
            RetriveMode::NormalMode
        );
    };
}

std::function<AbstractGameAutomation *()> PlayingScene::hard_increment_automate()
{
    std::uniform_int_distribution<int> uni_gen(
        51 - (50 + this->parent->client->consumer_level())/(this->parent->client->consumer_level() + 1),
        101 - (100 + this->parent->client->consumer_level())/(this->parent->client->consumer_level() + 1)
    );
    auto uni_rand = std::bind(uni_gen, std::mt19937(static_cast<unsigned int>(time(nullptr)) * 23333U + 3U));

    if (game_config != nullptr) {
        delete game_config;
    }
    game_config = new GameConfig(GameConfigMode::HARD_HARDMODE_CONFIG);

    return [this, uni_rand]() mutable -> AbstractGameAutomation * {
        return this->parent->client->start_game_event(
            game_config,
            std::max(1, static_cast<int>(uni_rand())) ,
            RetriveMode::HardMode
        );
    };
}

//GameAutomation *PlayingScene::easy_increment_automate()
//{


//}


std::function<void()> PlayingScene::single_round(const std::function<AbstractGameAutomation*()> &moder)
{
    return [this, moder]() mutable {

        auto automate = moder();

        if (automate == nullptr) {
            MessageBox::critical(this, "错误", this->parent->client->last_error());
            return;
        }

        automate->setParent(this);

        connect(automate, &AbstractGameAutomation::start_game, [this]() mutable {
            qDebug() << "start game";
            this->parent->switch_scene(this->parent->playsub_scene);
        });
        connect(automate, &AbstractGameAutomation::new_tango, [this](TangoPair tango, int fade_time) mutable {
            qDebug() << "retreving" << tango;
            this->parent->playsub_scene->key_label->setText(tango.first);
            this->parent->playsub_scene->value_label->setText(tango.second);
            this->parent->playsub_scene->key_label->show();
            this->parent->playsub_scene->value_label->show();


            this->parent->playsub_scene->user_ret->hide();
            this->parent->playsub_scene->answer_button->hide();

            this->parent->timer->set_timer(fade_time);
        });
        connect(automate, &GameAutomation::tango_faded, [this](int ans_time) mutable {
            qDebug() << "tango faded";
            this->parent->playsub_scene->key_label->hide();
            this->parent->playsub_scene->value_label->hide();

            this->parent->playsub_scene->user_ret->show();
            this->parent->playsub_scene->answer_button->show();

            this->parent->timer->set_timer(ans_time);
        });
        connect(automate, &AbstractGameAutomation::answer_failed, []() mutable {
            qDebug() << "answer failed";
        });
        connect(automate, &AbstractGameAutomation::success, [this, automate]() mutable {
            qDebug() << "success";
            this->settle_game(automate);
        });
        connect(automate, &AbstractGameAutomation::failed, [this, automate]() mutable {
            qDebug() << "failed";
            this->settle_game(automate);
        });
        connect(this->parent->playsub_scene->answer_button, &QPushButton::clicked, [this, automate]() mutable {

            if (this->parent->playsub_scene->user_ret->text() == this->parent->playsub_scene->key_label->text()) {
                automate->answer_tango(this->parent->playsub_scene->user_ret->text());
                this->parent->playsub_scene->user_ret->setText("");
            }

        });
        connect(this->parent->playsub_scene->stop_button, &QPushButton::clicked, [this, automate]() mutable {
            automate->stop();
            this->parent->switch_scene(this->parent->playset_scene);
        });

        automate->start();
    };
}

std::function<void()> PlayingScene::single_round_must_done(const std::function<AbstractGameAutomation*()> &moder)
{
    return [this, moder]() mutable {

        auto automate = moder();

        if (automate == nullptr) {
            MessageBox::critical(this, "错误", this->parent->client->last_error());
            return;
        }

        automate->setParent(this);

        connect(automate, &AbstractGameAutomation::start_game, [this]() mutable {
            qDebug() << "start game";
            this->parent->switch_scene(this->parent->playsub_scene);
        });
        connect(automate, &AbstractGameAutomation::new_tango, [this](TangoPair tango, int fade_time) mutable {
            qDebug() << "retreving" << tango;
            this->parent->playsub_scene->key_label->setText(tango.first);
            this->parent->playsub_scene->value_label->setText(tango.second);
            this->parent->playsub_scene->key_label->show();
            this->parent->playsub_scene->value_label->show();


            this->parent->playsub_scene->user_ret->hide();
            this->parent->playsub_scene->answer_button->hide();

            this->parent->timer->set_timer(fade_time);
        });
        connect(automate, &AbstractGameAutomation::tango_faded, [this](int ans_time) mutable {
            qDebug() << "tango faded";
            this->parent->playsub_scene->key_label->hide();
            this->parent->playsub_scene->value_label->hide();

            this->parent->playsub_scene->user_ret->show();
            this->parent->playsub_scene->answer_button->show();

            this->parent->timer->set_timer(ans_time);
        });
        connect(automate, &AbstractGameAutomation::answer_failed, []() mutable {
            qDebug() << "answer failed";
        });
        connect(automate, &AbstractGameAutomation::success, [this, automate]() mutable {
            qDebug() << "success";
            this->settle_game(automate);
        });
        connect(automate, &AbstractGameAutomation::failed, [this, automate]() mutable {
            qDebug() << "failed";
            this->abort_game(automate);
        });
        connect(this->parent->playsub_scene->answer_button, &QPushButton::clicked, [this, automate]() mutable {

            if (this->parent->playsub_scene->user_ret->text() == this->parent->playsub_scene->key_label->text()) {
                automate->answer_tango(this->parent->playsub_scene->user_ret->text());
                this->parent->playsub_scene->user_ret->setText("");
            }

        });
        connect(this->parent->playsub_scene->stop_button, &QPushButton::clicked, [this, automate]() mutable {
            automate->stop();
            this->parent->switch_scene(this->parent->playset_scene);
        });

        automate->start();
    };
}


PlayingScene::PlayingScene(QWidget *parent): Scene (parent)
{
    this->parent = dynamic_cast<MainWindow*>(parent);
    game_config = nullptr;

    trial_button = new QPushButton;
    trial_button->setText("体验");

    easy_button = new QPushButton;
    easy_button->setText("简单");

    normal_button = new QPushButton;
    normal_button->setText("普通");

    hard_button = new QPushButton;
    hard_button->setText("困难");

    center_lay = new QVBoxLayout;
    center_lay->addWidget(trial_button);
    center_lay->addWidget(easy_button);
    center_lay->addWidget(normal_button);
    center_lay->addWidget(hard_button);

    connect(trial_button, &QPushButton::clicked, [this]() mutable {
        this->parent->playset_scene->from_exp->setNum(this->parent->client->consumer_exp());
        this->parent->playset_scene->from_level->setNum(this->parent->client->consumer_level());
        this->single_round(this->default_automate())();
    });

    connect(easy_button, &QPushButton::clicked, [this]() mutable {
        this->parent->playset_scene->from_exp->setNum(this->parent->client->consumer_exp());
        this->parent->playset_scene->from_level->setNum(this->parent->client->consumer_level());
        this->single_round_must_done(this->easy_increment_automate())();
    });

    connect(normal_button, &QPushButton::clicked, [this]() mutable {
        this->parent->playset_scene->from_exp->setNum(this->parent->client->consumer_exp());
        this->parent->playset_scene->from_level->setNum(this->parent->client->consumer_level());
        this->single_round_must_done(this->normal_increment_automate())();
    });
    connect(hard_button, &QPushButton::clicked, [this]() mutable {
        this->parent->playset_scene->from_exp->setNum(this->parent->client->consumer_exp());
        this->parent->playset_scene->from_level->setNum(this->parent->client->consumer_level());
        this->single_round_must_done(this->hard_increment_automate())();
    });



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

void PlayingScene::settle_game(AbstractGameAutomation *automate)
{
    qDebug() << "setting...";
    disconnect(this->parent->playsub_scene->answer_button, nullptr, nullptr, nullptr);
    disconnect(this->parent->playsub_scene->stop_button, nullptr, nullptr, nullptr);
    this->parent->timer->stop_timer();

    int origin_level = this->parent->playset_scene->from_exp->text().toInt();

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
    if (!this->parent->client->sync_status()) {
        qDebug() << "同步失败..." << this->parent->client->last_error();
    }
    this->parent->switch_scene(this->parent->playset_scene);
    this->parent->playsub_scene->user_ret->setText("");
}

void PlayingScene::abort_game(AbstractGameAutomation *automate)
{
    qDebug() << "abort setting...";
    disconnect(this->parent->playsub_scene->answer_button, nullptr, nullptr, nullptr);
    disconnect(this->parent->playsub_scene->stop_button, nullptr, nullptr, nullptr);
    this->parent->timer->stop_timer();

    int origin_level = this->parent->playset_scene->from_exp->text().toInt();

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
