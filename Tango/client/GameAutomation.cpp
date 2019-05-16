#include "GameAutomation.h"
#include "GameClient.h"

#include <QDebug>

GameAutomation::GameAutomation(GameClient *client, QObject *parent) : QObject(parent)
{
    this->client = client;
    qDebug() << this->client->is_connected();
}
