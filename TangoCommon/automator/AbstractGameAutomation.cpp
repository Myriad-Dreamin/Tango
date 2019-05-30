#include "AbstractGameAutomation.h"

AbstractGameAutomation::AbstractGameAutomation(QObject *parent) : QObject(parent)
{

}

const QString AbstractGameAutomation::last_error()
{
    return _last_error;
}

AbstractGameAutomation::~AbstractGameAutomation()
{

}
