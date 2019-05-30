#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <functional>
#include <vector>
#include "../types/TangoPair.h"

const int DEFALUT_FADE_TIME = 5000, DEFAULT_ANS_TIME = 10000;

class GameAutomation;

void default_fade_functor(int &x);
void default_ans_functor(int &x);

void decreasing_fade_functor(int &x);
void decreasing_ans_functor(int &x);

/* 返回一个第一次调用返回false, 以后都返回true的函数. */
std::function<bool()> second_chance_functor_gen();

void default_exp_functor(int &x, TangoPair tango, int level_step);
void easy_exp_increment(int &exp, TangoPair tango, int success_count);
void normal_exp_increment(int &exp, TangoPair tango, int success_count);
void hard_exp_increment(int &exp, TangoPair tango, int success_count);


bool default_failed_functor();

/* 游戏参数设定类 */
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
    /* 是否使用外部单词池作为存储 */
    bool from_out_pool;
    /* 是否开启tick事件 */
    bool enable_elasped;
    /* 单词池 */
    std::vector<TangoPair> *tango_pool;
    /* 单词消失时间 */
    int fade_time;
    /* 回答时间 */
    int ans_time;
    /* 消失时间变换函子, 每次单词作答完以后作用一次 */
    std::function<void(int&)> fade_functor;
    /* 回答时间变换函子, 每次单词作答完以后作用一次 */
    std::function<void(int&)> ans_functor;
    /* 经验计算函子, 传入一个单词和当前轮数作为参考 */
    std::function<void(int&,TangoPair,int)> exp_functor;
    /* 每次失败事件触发以后, 调用失败函数函子判断是否仍有机会 */
    std::function<bool()> if_failed_functor;
};

namespace GameConfigMode {
    /* 默认设定 */
    const GameConfig DEFAULT_CONFIG = GameConfig();

    /* 每轮时间递减 */
    const GameConfig DECREASE_TIME_CONFIG = GameConfig(
        false,
        decreasing_fade_functor,
        decreasing_ans_functor
    );

    const GameConfig HARD_EASYMODE_CONFIG = GameConfig(
        false,
        decreasing_fade_functor,
        decreasing_ans_functor,
        second_chance_functor_gen(),
        easy_exp_increment
    );

    const GameConfig HARD_NORMALMODE_CONFIG = GameConfig(
        false,
        decreasing_fade_functor,
        decreasing_ans_functor,
        second_chance_functor_gen(),
        easy_exp_increment
    );

    const GameConfig HARD_HARDMODE_CONFIG = GameConfig(
        false,
        decreasing_fade_functor,
        decreasing_ans_functor,
        second_chance_functor_gen(),
        easy_exp_increment
    );

}


#endif // GAMECONFIG_H
