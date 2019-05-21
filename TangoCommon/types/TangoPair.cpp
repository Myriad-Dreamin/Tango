#include "TangoPair.h"

TangoPair::TangoPair(QString first_pair, QString second_pair):
    std::pair<QString, QString> (first_pair, second_pair)
{

}

QJsonArray TangoPair::to_json_array(const TangoPair &tango)
{
    QJsonArray ret;
    ret.push_back(tango.first);
    ret.push_back(tango.second);
    return ret;
}

TangoPair TangoPair::from_json_array(const QJsonValue first, const QJsonValue second)
{
    return TangoPair(first.toString(), second.toString());
}

TangoPair TangoPair::from_json_array(QJsonArray tango)
{
    switch (tango.size()) {
    case 0:
        return TangoPair("", "");
    case 1:
        return TangoPair(tango[0].toString(), "");
    default:
        return TangoPair(tango[0].toString(), tango[1].toString());
    }
}
