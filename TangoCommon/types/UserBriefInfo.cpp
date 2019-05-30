#include "UserBriefInfo.h"

UserBriefInfo::UserBriefInfo() {}

UserBriefInfo::UserBriefInfo(int id, QString name, int level): id(id), name(name), level(level){}

QJsonArray UserBriefInfo::to_json_array(const UserBriefInfo &info)
{
    QJsonArray ret;

    ret.push_back(info.id);
    ret.push_back(info.name);
    ret.push_back(info.level);

    return ret;
}

UserBriefInfo UserBriefInfo::from_json_array(const QJsonArray &info)
{
    UserBriefInfo ret;
    ret.id = info[0].toInt();
    ret.name = info[1].toString();
    ret.level = info[2].toInt();
    return ret;
}
