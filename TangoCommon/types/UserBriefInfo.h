#ifndef USERBRIEFINFO_H
#define USERBRIEFINFO_H

#include <QString>
#include <QJsonArray>

class UserBriefInfo
{
public:
    UserBriefInfo();
    UserBriefInfo(int id, QString name, int level);

    int id;
    QString name;
    int level;

    static QJsonArray to_json_array(const UserBriefInfo &info);
    static UserBriefInfo from_json_array(const QJsonArray &info);
};

#endif // USERBRIEFINFO_H
