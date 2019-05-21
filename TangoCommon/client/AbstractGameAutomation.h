#ifndef ABSTRACTGAMEAUTOMATION_H
#define ABSTRACTGAMEAUTOMATION_H

#include <QObject>
#include <functional>
#include "GameConfig.h"
#include <QTimer>
#include "../types/TangoPair.h"

class GameClient;
class TangoPair;

class AbstractGameAutomation : public QObject
{
    Q_OBJECT
    friend class Client;
    friend class AbstractClient;
    friend class LocalClient;
public:
    explicit AbstractGameAutomation(QObject *parent = nullptr);
    virtual ~AbstractGameAutomation() = 0;
    virtual const QString last_error() = 0;
    virtual bool set_config(const GameConfig *game_config) = 0;
    virtual bool start() = 0;
    virtual bool prepare_start(const std::vector<TangoPair> &tango_list, unsigned int n) = 0;
public:
// c++ style public slots:
    virtual std::function<void ()> make_answer_success_slotter() = 0;
    virtual std::function<void ()> make_stop_slotter() = 0;
signals:
    void start_game();
    void elasped();
    void new_tango(TangoPair);
    void tango_faded();
    void answer_failed();
    void success();
    void failed();
protected:
    bool prepared;
    std::vector<TangoPair> *tango_pool;
    int fade_time, ans_time, success_count;
    int exp;
    unsigned int select_ptr;
    const GameConfig *config;
    QString _last_error;
    QTimer *timer, *elasped_timer;

};

#endif // ABSTRACTGAMEAUTOMATION_H
