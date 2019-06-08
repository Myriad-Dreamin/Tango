#include "Player.h"

Player::Player(QSqlDatabase db_handler, QObject *parent) : QObject(parent)
{
    handler = db_handler;
}


Player::Player(QObject *parent) : QObject(parent)
{
}

Player::Player(const Player &rpl) :QObject (rpl.parent())
{
    this->handler = rpl.handler;
}

void Player::set_handler(const QSqlDatabase &handler)
{
    this->handler = handler;
}
