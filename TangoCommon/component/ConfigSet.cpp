#include "ConfigSet.h"

ConfigSet::ConfigSet(const QString &fileName, Format format, QObject *parent):
    QSettings (fileName, format, parent), std::map<QString, QVariant>()
{

}

bool ConfigSet::register_value(const QString &key, const QVariant &value)
{

}
