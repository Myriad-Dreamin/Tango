#include "ConfigSet.h"
#include <QDebug>


ConfigSet::ConfigSet(const QString &fileName, Format format, QObject *parent):
    QSettings (fileName, format, parent)
{

}

bool ConfigSet::register_value(const QString &key, const QVariant &value)
{
    static QVariant invalid_element(QVariant::Type::Invalid);
    if (default_set.count(key)) {
        return false;
    }
    default_set[key] = value;
    if (!this->value(key, invalid_element).isValid()) {
        this->setValue(key, value);
    }
    return true;
}

bool ConfigSet::set_default_value(const QString &key, const QVariant &value)
{
    static QVariant invalid_element(QVariant::Type::Invalid);
    default_set[key] = value;
    qDebug() << "here" << key << value;
    if (!this->value(key, invalid_element).isValid()) {
        qDebug() << "here" << key << value;
        this->setValue(key, value);
    }
    return true;
}

QVariant ConfigSet::operator[](const QString &key)
{
    static QVariant invalid_element(QVariant::Type::Invalid);
    QVariant to_get = this->value(key, invalid_element);
    if (to_get.isValid()) {
        return to_get;
    }
    if (default_set.count(key)) {
        return default_set[key];
    }
    return invalid_element;
}


const QVariant & ConfigSet::at(const QString &key)
{
    static QVariant invalid_element(QVariant::Type::Invalid);
    if (this->value(key, invalid_element).isValid()) {
        default_set[key] = this->value(key, invalid_element);
    }
    if (default_set.count(key)) {
        return default_set.at(key);
    }
    return invalid_element;
}
