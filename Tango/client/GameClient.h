#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QString>

class GameClient
{
public:
    // virtual ~GameClient() = 0;
    // virtual bool author_sign_in(QString account, QString password) = 0;
    virtual bool is_connected() = 0;
};

#endif // GAMECLIENT_H
