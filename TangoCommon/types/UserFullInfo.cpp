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

QJsonArray UserFullInfo::to_json_array(const UserFullInfo &info)
{
    QJsonArray ret;
    ret.push_back(info.user_id);
    ret.push_back(info.tango_count);
    ret.push_back(info.level);
    ret.push_back(info.misson_count);
    ret.push_back(info.exp);
    ret.push_back(info.name);
    ret.push_back(info.motto);
    return ret;
}

UserFullInfo UserFullInfo::from_json_array(const QJsonArray &arr, const int offset)
{
    UserFullInfo ret;
    ret.user_id = arr[offset + 0].toInt();
    ret.tango_count = arr[offset + 1].toInt();
    ret.level = arr[offset + 2].toInt();
    ret.misson_count = arr[offset + 3].toInt();
    ret.exp = arr[offset + 4].toInt();
    ret.name = arr[offset + 5].toString();
    ret.motto = arr[offset + 6].toString();
    return ret;
}
