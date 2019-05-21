#ifndef TANGOPAIR_H
#define TANGOPAIR_H
#include <utility>
#include <QString>
#include <QJsonArray>

class TangoPair : public std::pair<QString, QString>
{
public:
    TangoPair(QString first_pair, QString second_pair);

    static TangoPair from_json_array(const QJsonValue first, const QJsonValue second);
    static TangoPair from_json_array(QJsonArray tango);
    static QJsonArray to_json_array(const TangoPair &tango);
};

#endif // TANGOPAIR_H
