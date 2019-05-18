#ifndef GAMEAUTOMATION_H
#define GAMEAUTOMATION_H

#include <QObject>
#include <functional>
#include "GameConfig.h"
#include <QTimer>
#include "../types/TangoPair.h"

class GameClient;
class TangoPair;

class GameAutomation : public QObject
{
    Q_OBJECT
    friend class Client;
public:
    explicit GameAutomation(const GameConfig *game_config=&DEFAULT_CONFIG, QObject *parent = nullptr);
    ~GameAutomation();
    const QString last_error();
    bool set_config(const GameConfig *game_config);
    bool start();
    bool prepare_start(const std::vector<TangoPair> &tango_list, unsigned int n);
public:
// c++ style public slots:
    std::function<void ()> make_answer_success_slotter();
    std::function<void ()> make_stop_slotter();
signals:
    void start_game();
    void elasped();
    void new_tango(TangoPair);
    void tango_faded();
    void answer_failed();
    void success();
    void failed();
private:
    GameClient *client;

    bool prepared;
    std::vector<TangoPair> *tango_pool;
    int fade_time, ans_time, success_count;
    int exp;
    unsigned int select_ptr;
    const GameConfig *config;
    QString _last_error;
    QTimer *timer, *elasped_timer;

//    std::function<void()> elasped_event;
//    std::function<void(TangoPair)> new_tango_event;
//    std::function<void()> tango_faded_event;
//    std::function<void()> answer_success_event;
//    std::function<void()> answer_failed_event;
//    std::function<void()> success_event;
//    std::function<void()> failed_event;

//    bool set_elasped_event(const std::function<void()> &ev);
//    bool set_new_tango_event(const std::function<void(TangoPair)> &ev);
//    bool set_tango_faded_event(const std::function<void()> &ev);
//    bool set_answer_success_event(const std::function<void()> &ev);
//    bool set_answer_failed_event(const std::function<void()> &ev);
//    bool set_success_event(const std::function<void()> &ev);
//    bool set_failed_event(const std::function<void()> &ev);

    bool prepare_tango_pool(const std::vector<TangoPair> &tango_list, unsigned int n);
    bool set_timeout_mode(
        const std::function<void (int &)> &fade_timearr_functor,
        const std::function<void (int &)> &ack_timearr_functor
    );
    inline bool setup_automation();
    void select_new_tango();
    void make_faded_event();
    inline void stop_automation();
    void clear_state();
};

#endif // GAMEAUTOMATION_H
