
#include "GameAutomation.h"

#include <vector>
#include <random>
#include <algorithm>
#include <ctime>


#include <QDebug>

#include "GameClient.h"

GameAutomation::GameAutomation(const GameConfig *game_config, QObject *parent): QObject(parent)
{
    this->config = game_config;
    this->tango_pool = this->config->tango_pool;
    this->fade_time = this->config->fade_time;
    this->ans_time = this->config->ans_time;
    this->select_ptr = 0;
    this->timer = new QTimer(this);
    this->elasped_timer = new QTimer(this);
    this->timer->setSingleShot(true);
    this->elasped_timer->setSingleShot(false);
    this->prepared = false;
}

GameAutomation::~GameAutomation()
{
    qDebug() << "deleted automation";
    this->stop_automation();
    delete this->timer;
    delete this->elasped_timer;
}

const QString GameAutomation::last_error()
{
    return _last_error;
}


bool GameAutomation::prepare_tango_pool(const std::vector<TangoPair> &tango_list, unsigned int n)
{
    std::mt19937 mtrand(static_cast<unsigned int>(time(nullptr)));

    if (n > tango_list.size()) {
        _last_error = "no enough tangos";
        return false;
    }

    this->tango_pool->clear();
    this->tango_pool->reserve(n);
    for(unsigned int i = mtrand() % tango_list.size();n--; i = (i + 1) % tango_list.size()) {
        this->tango_pool->push_back(tango_list[i]);
    }
    std::random_shuffle(this->tango_pool->begin(), this->tango_pool->end());

    return true;
}

bool GameAutomation::set_config(const GameConfig *game_config) {
    this->config = game_config;
    return true;
}

bool GameAutomation::start()
{
    if (!this->prepared) {
        return false;
    }

    this->setup_automation();
    if (this->config->enable_elasped) {
        connect(this->elasped_timer, &QTimer::timeout, [this]() mutable {
            emit this->elasped();
        });
        this->elasped_timer->start(100);
        qDebug() << "timer";
    }
    emit this->start_game();

    return true;
}

bool GameAutomation::prepare_start(const std::vector<TangoPair> &tango_list, unsigned int n)
{
    if (!this->prepare_tango_pool(tango_list, n)) {
        return false;
    }
    this->select_ptr = 0;
    this->prepared = true;
    return true;
}

//bool GameAutomation::set_elasped_event(const std::function<void ()> &ev)
//{
//    this->elasped_event = ev;
//    return false;
//}

inline bool GameAutomation::setup_automation()
{
    connect(this, &GameAutomation::start_game, [this]() mutable {
        this->select_new_tango();
    });
    connect(this, &GameAutomation::success, [this]() mutable {
        this->stop_automation();
    });
    connect(this, &GameAutomation::failed, [this]() mutable {
        this->stop_automation();
    });

    return true;
}

void GameAutomation::select_new_tango()
{
    qDebug() << "new tango" << this->select_ptr << this->tango_pool->size();
    if (this->select_ptr >= this->tango_pool->size()) {
        emit this->success();
    } else {
        qDebug() << "selecting " << (*this->tango_pool)[this->select_ptr];
        emit this->new_tango((*this->tango_pool)[this->select_ptr++]);

        disconnect(this->timer, nullptr, nullptr, nullptr);
        connect(this->timer, &QTimer::timeout, [this]() mutable {
            qDebug() << "fade" << this->fade_time;
            this->make_faded_event();
        });
        this->timer->start(this->fade_time);

        this->config->fade_functor(this->fade_time);
    }
}

void GameAutomation::make_faded_event()
{
    emit this->tango_faded();
    disconnect(this->timer, nullptr, nullptr, nullptr);
    connect(this->timer, &QTimer::timeout, [this]() mutable {
        emit this->answer_failed();
        if (this->config->if_failed_functor()) {
            emit this->failed();
        } else {
            this->select_ptr--;
            this->select_new_tango();
        }
    });
    this->timer->start(this->ans_time);
}

std::function<void()> GameAutomation::make_answer_success_slotter()
{
    return [this]() mutable {
        this->config->exp_functor(this->exp, (*this->tango_pool)[this->select_ptr - 1], static_cast<int>(this->select_ptr));
        select_new_tango();
    };
}

std::function<void()> GameAutomation::make_stop_slotter()
{
    return [this]() mutable {
        this->stop_automation();
        this->deleteLater();
    };
}

//std::function<void()> GameAutomation::make_pause_slotter()
//{
//    return [this]() mutable {
//        athis->timer->
//    };
//}

inline void GameAutomation::stop_automation() {
    qDebug() << "stop automation";
    disconnect(this->timer, nullptr, nullptr, nullptr);
    disconnect(this->elasped_timer, nullptr, nullptr, nullptr);
    this->timer->stop();
    this->elasped_timer->stop();
    this->prepared = false;
    this->select_ptr = 0;
}
