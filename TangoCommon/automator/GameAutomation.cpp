
#include "GameAutomation.h"

#include <vector>
#include <random>
#include <algorithm>
#include <ctime>


#include <QDebug>

#include "GameClient.h"

GameAutomation::GameAutomation(const GameConfig *game_config, QObject *parent): AbstractGameAutomation(parent)
{
    this->clear_state();
    this->set_config(game_config);

    this->timer = new QTimer(this);
    this->elasped_timer = new QTimer(this);
    this->timer->setSingleShot(true);
    this->elasped_timer->setSingleShot(false);
}

GameAutomation::~GameAutomation()
{
    qDebug() << "deleted automation" << this;
    this->stop_automation();
    delete this->timer;
    delete this->elasped_timer;
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
    this->tango_pool = game_config->tango_pool;
    this->fade_time = game_config->fade_time;
    this->ans_time = game_config->ans_time;
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
            emit AbstractGameAutomation::elasped();
        });
        this->elasped_timer->start(100);
        qDebug() << "timer";
    }
    emit AbstractGameAutomation::start_game();

    return true;
}

bool GameAutomation::stop()
{
    emit AbstractGameAutomation::failed();
    return true;
}

bool GameAutomation::prepare_start(const std::vector<TangoPair> &tango_list, unsigned int n)
{
    this->clear_state();
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
        emit AbstractGameAutomation::success();
    } else {
        qDebug() << "selecting " << (*this->tango_pool)[this->select_ptr];
        emit AbstractGameAutomation::new_tango((*this->tango_pool)[this->select_ptr++], this->fade_time);

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
    emit AbstractGameAutomation::tango_faded(this->ans_time);
    disconnect(this->timer, nullptr, nullptr, nullptr);
    connect(this->timer, &QTimer::timeout, [this]() mutable {
        emit AbstractGameAutomation::answer_failed();
        if (this->config->if_failed_functor()) {
            emit AbstractGameAutomation::failed();
        } else {
            this->select_ptr--;
            this->select_new_tango();
        }
    });
    this->timer->start(this->ans_time);

    this->config->ans_functor(this->ans_time);
}


void GameAutomation::answer_tango(QString t)
{
    if ((*tango_pool)[static_cast<unsigned int>(this->success_count)].first == t) {
        this->success_count++;
        qDebug() << "expfunction..." << reinterpret_cast<unsigned long long>(&this->config->exp_functor);
        this->config->exp_functor(this->exp, (*this->tango_pool)[this->select_ptr - 1], static_cast<int>(this->select_ptr));
        select_new_tango();
    }
}

//std::function<void()> GameAutomation::make_pause_slotter()
//{
//    return [this]() mutable {
//        athis->timer->
//    };
//}

inline void GameAutomation::stop_automation() {
    qDebug() << "stop automation" << this->select_ptr;
    disconnect(this->timer, nullptr, nullptr, nullptr);
    disconnect(this->elasped_timer, nullptr, nullptr, nullptr);
    this->timer->stop();
    this->elasped_timer->stop();
}

inline void GameAutomation::clear_state()
{
    this->prepared = false;
    this->select_ptr = 0;
    this->success_count = 0;
    this->exp = 0;
}

