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
    const QString last_error();
    virtual bool start() = 0;
    virtual bool stop() = 0;
public:
// c++ style public slots:
    virtual void answer_tango(QString) = 0;
signals:
    void start_game();
    void elasped();
    void new_tango(TangoPair,int);
    void tango_faded(int);
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
