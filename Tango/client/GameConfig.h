#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <functional>
#include <vector>
#include "../types/TangoPair.h"

class GameAutomation;

void default_fade_functor(int &x);
void default_ans_functor(int &x);

void decreasing_fade_functor(int &x);
void decreasing_ans_functor(int &x);

std::function<void()> second_chance_functor_gen(int &x);

void default_exp_functor(int &x, TangoPair tango, int level_step);
bool default_failed_functor();
const int DEFALUT_FADE_TIME = 5000, DEFAULT_ANS_TIME = 10000;

class GameConfig
{
    friend class GameAutomation;

public:
    GameConfig(
        bool enable_elasped=false,
        const std::function<void(int&)> &fade_functor=default_fade_functor,
        const std::function<void(int&)> &ans_functor=default_ans_functor,
        const std::function<bool()> &if_failed_functor=default_failed_functor,
        const std::function<void(int&,TangoPair,int)> &exp_functor=default_exp_functor,
        const int fade_time=DEFALUT_FADE_TIME,
        const int ans_time=DEFAULT_ANS_TIME
    );
    GameConfig(
        std::vector<TangoPair> &stable_tango_pool,
        bool enable_elasped=false,
        const std::function<void(int&)> &fade_functor=default_fade_functor,
        const std::function<void(int&)> &ans_functor=default_ans_functor,
        const std::function<bool()> &if_failed_functor=default_failed_functor,
        const std::function<void(int&,TangoPair,int)> &exp_functor=default_exp_functor,
        const int fade_time=DEFALUT_FADE_TIME,
        const int ans_time=DEFAULT_ANS_TIME
    );
    GameConfig(GameConfig &&game_config);
    GameConfig(const GameConfig &game_config);
    ~GameConfig();

private:
    bool from_out_pool, enable_elasped;
    std::vector<TangoPair> *tango_pool;
    int fade_time, ans_time;
    std::function<void(int&)> fade_functor;
    std::function<void(int&)> ans_functor;
    std::function<void(int&,TangoPair,int)> exp_functor;
    std::function<bool()> if_failed_functor;

};

const GameConfig DEFAULT_CONFIG = GameConfig();
const GameConfig DECREASE_TIMECONFIG = GameConfig(
    false,
    decreasing_fade_functor,
    decreasing_ans_functor
);


#endif // GAMECONFIG_H
