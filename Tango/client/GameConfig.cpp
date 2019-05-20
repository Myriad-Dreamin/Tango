#include "GameConfig.h"

#include <algorithm>

#include <QDebug>

#include "../types/TangoPair.h"


GameConfig::GameConfig(
    bool enable_elasped,
    const std::function<void(int &)> &fade_functor,
    const std::function<void(int &)> &ans_functor,
    const std::function<bool()> &if_failed_functor,
    const std::function<void(int&,TangoPair,int)> &exp_functor,
    const int fade_time,
    const int ans_time
) {
    this->tango_pool = new std::vector<TangoPair>;
    this->from_out_pool = false;

    this->enable_elasped = enable_elasped;

    this->fade_functor = fade_functor;
    this->ans_functor = ans_functor;
    this->if_failed_functor = if_failed_functor;
    this->exp_functor = exp_functor;

    this->fade_time = fade_time;
    this->ans_time = ans_time;
}

GameConfig::GameConfig(
    std::vector<TangoPair> &stable_tango_pool,
    bool enable_elasped,
    const std::function<void(int &)> &fade_functor,
    const std::function<void(int &)> &ans_functor,
    const std::function<bool()> &if_failed_functor,
    const std::function<void(int&,TangoPair,int)> &exp_functor,
    const int fade_time,
    const int ans_time
) {
    this->tango_pool = &stable_tango_pool;
    this->from_out_pool = true;

    this->enable_elasped = enable_elasped;

    this->fade_functor = fade_functor;
    this->ans_functor = ans_functor;
    this->if_failed_functor = if_failed_functor;
    this->exp_functor = exp_functor;

    this->fade_time = fade_time;
    this->ans_time = ans_time;
}


GameConfig::GameConfig(const GameConfig &game_config)
{
    if (game_config.from_out_pool) {
        this->tango_pool = game_config.tango_pool;
    } else {
        this->tango_pool = new std::vector<TangoPair>;
    }
    this->from_out_pool = game_config.from_out_pool;

    this->enable_elasped = game_config.enable_elasped;

    this->fade_functor = game_config.fade_functor;
    this->if_failed_functor = game_config.if_failed_functor;
    this->if_failed_functor = game_config.if_failed_functor;
    this->exp_functor = game_config.exp_functor;

    this->fade_time = game_config.fade_time;
    this->ans_time = game_config.ans_time;
}

GameConfig::GameConfig(GameConfig &&game_config)
{
    this->tango_pool = game_config.tango_pool;
    game_config.tango_pool = nullptr;

    this->from_out_pool = game_config.from_out_pool;
    game_config.from_out_pool = true;

    this->enable_elasped = game_config.enable_elasped;

    this->fade_functor = std::move(game_config.fade_functor);
    this->ans_functor = std::move(game_config.ans_functor);
    this->if_failed_functor = std::move(game_config.if_failed_functor);
    this->exp_functor = std::move(game_config.exp_functor);

    this->fade_time = game_config.fade_time;
    this->ans_time = game_config.ans_time;
}
GameConfig::~GameConfig()
{
    if (this->from_out_pool == false) {
        delete this->tango_pool;
        this->tango_pool = nullptr;
    }
}

void default_fade_functor(int&)
{
    return;
}

void decreasing_fade_functor(int& x)
{
    x = std::max(1000, x - 50);
    return;
}


void default_ans_functor(int&)
{
    return;
}

void decreasing_ans_functor(int& x)
{
    x = std::max(1000, x - 50);
    return;
}

bool default_failed_functor()
{
    return true;
}

std::function<void()> second_chance_functor_gen()
{
    bool change_count = 1;
    return [change_count]() mutable {
        if (change_count) {
            change_count = false;
            return false;
        } else {
            return true;
        }
    };
}


void default_exp_functor(int &x, TangoPair tango, int)
{
    qDebug() << x << tango;
    x += tango.first.length();
    return;
}
