#ifndef CONFIGSET_H
#define CONFIGSET_H

#include <QSettings>


class ConfigSet : public QSettings, public std::map<QString, QVariant>
{
    Q_OBJECT
public:
    ConfigSet(const QString &fileName, Format format, QObject *parent = nullptr);
    bool register_value(const QString &key, const QVariant &value);
};

#endif // CONFIGSET_H
