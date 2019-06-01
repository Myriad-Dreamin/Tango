#ifndef CONFIGSET_H
#define CONFIGSET_H

#include <QSettings>


class ConfigSet : public QSettings
{
    Q_OBJECT
    std::map<QString, QVariant> default_set;
public:
    ConfigSet(const QString &fileName, Format format, QObject *parent = nullptr);
    bool register_value(const QString &key, const QVariant &value);
    bool set_default_value(const QString &key, const QVariant &value);
    const QVariant &get_default_value(const QString &key);
    bool reset();

    const QVariant operator[](const QString & key);
    const QVariant at(const QString & key);
};

#endif // CONFIGSET_H
