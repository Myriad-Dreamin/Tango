#ifndef USERFULLINFO_H
#define USERFULLINFO_H

#include <QString>

class UserFullInfo
{
public:
    UserFullInfo();
    UserFullInfo(int user_id, QString name, int tango_count, int misson_count, int level, int exp, QString motto);
    int user_id;
    QString name;

    int tango_count;
    int misson_count;

    int level;
    int exp;

    QString motto;
};

#endif // USERFULLINFO_H
