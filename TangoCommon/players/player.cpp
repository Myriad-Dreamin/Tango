#include "Player.h"

Player::Player(QSqlDatabase db_handler, QObject *parent) : QObject(parent)
{
    handler = db_handler;
}


Player::Player(QObject *parent) : QObject(parent)
{
}
