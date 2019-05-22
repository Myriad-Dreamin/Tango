#ifndef GAMEAUTOMATIONRELAYER_H
#define GAMEAUTOMATIONRELAYER_H

#include <QObject>
#include <functional>
#include "GameConfig.h"
#include <QTimer>
#include "AbstractGameAutomation.h"
#include "../types/TangoPair.h"

class TangoPair;
class RemoteClient;

class GameAutomationRelayer : public AbstractGameAutomation
{
    Q_OBJECT
    friend class Client;
    friend class AbstractClient;
    friend class LocalClient;
    friend class RemoteClient;
public:
    explicit GameAutomationRelayer(RemoteClient *client, QObject *parent = nullptr);
    ~GameAutomationRelayer();

    bool start();
    bool stop();

signals:
    void answering(QString tango);
    void aborted();

public:
// c++ style public slots:

    void answer_tango(QString tango);
    std::function<void ()> make_stop_slotter();
private:
    RemoteClient *client;


    inline void stop_automation();
    void emit_start_game();
    void emit_elasped();
    void emit_new_tango(TangoPair,int);
    void emit_tango_faded(int);
    void emit_answer_failed();
    void emit_success();
    void emit_failed();

    void set_error(QString error);
};

#endif // GAMEAUTOMATIONRELAYER_H
