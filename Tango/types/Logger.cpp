#include "Logger.h"
#include <QDebug>
#include <QMessageLogger>


Logger::Logger(QObject *parent) : QObject(parent)
{
    this->log_flag = 0;
}

DEBUGRET Logger::debug()
{
    #if DEBUGLEVEL >= DEBUGFLAG
    return qDebug();
    # else
    return QNoDebug();
    # endif
}

WARNINGRET Logger::warning()
{
    #if DEBUGLEVEL >= WARNINGFLAG
    return qDebug();
    # else
    return QNoDebug();
    # endif
}

INFORET Logger::info()
{
    #if DEBUGLEVEL >= INFOFLAG
    return qDebug();
    # else
    return QNoDebug();
    # endif
}
