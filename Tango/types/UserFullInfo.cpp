#include "UserFullInfo.h"

UserFullInfo::UserFullInfo() {
    this->user_id = 0;
    this->tango_count = 0;
    this->level = 0;
    this->misson_count = 0;
    this->exp = 0;

    this->name = "";
    this->motto = "";
}

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
