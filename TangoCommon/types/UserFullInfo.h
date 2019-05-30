#ifndef USERFULLINFO_H
#define USERFULLINFO_H

#include <QString>
#include <QJsonArray>

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
    static QJsonArray to_json_array(const UserFullInfo &info);
    static UserFullInfo from_json_array(const QJsonArray &arr, const int offset=0);
};

#endif // USERFULLINFO_H
