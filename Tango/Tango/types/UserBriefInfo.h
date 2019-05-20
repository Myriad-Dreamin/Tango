#ifndef USERBRIEFINFO_H
#define USERBRIEFINFO_H

#include <QString>

class UserBriefInfo
{
public:
    UserBriefInfo(int id, QString name, int level);

    int id;
    QString name;
    int level;
};

#endif // USERBRIEFINFO_H
