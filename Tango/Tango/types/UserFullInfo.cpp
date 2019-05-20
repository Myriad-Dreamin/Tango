#include "UserFullInfo.h"

UserFullInfo::UserFullInfo() {}

UserFullInfo::UserFullInfo(
    int user_id,
    QString name,
    int tango_count,
    int misson_count,
    int level,
    int exp,
    QString motto
):
    user_id(user_id),
    name(name),
    tango_count(tango_count),
    misson_count(misson_count),
    level(level),
    exp(exp),
    motto(motto)
{}
