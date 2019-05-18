#ifndef TANGOPAIR_H
#define TANGOPAIR_H

#include <utility>
#include <QString>


class TangoPair: public std::pair<QString, QString>
{
public:
    explicit TangoPair(QString first_pair, QString second_pair);
};

#endif // TANGOPAIR_H
