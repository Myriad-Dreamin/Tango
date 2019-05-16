#ifndef GAMEAUTOMATION_H
#define GAMEAUTOMATION_H

#include <QObject>

class GameClient;

class GameAutomation : public QObject
{
    Q_OBJECT
public:
    explicit GameAutomation(GameClient *client, QObject *parent = nullptr);
private:
    GameClient *client;
};

#endif // GAMEAUTOMATION_H
