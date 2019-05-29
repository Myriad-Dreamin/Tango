#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QString>

/*
 * 游戏客户端
 * 
 * 已废弃
 */
class GameClient
{
public:
    virtual ~GameClient();
    // virtual bool author_sign_in(QString account, QString password) = 0;
    virtual bool is_connected() = 0;
};

#endif // GAMECLIENT_H
