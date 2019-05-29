#ifndef LOGGER_H
#define LOGGER_H

#define INFOFLAG 60
#define WARNINGFLAG 50
#define DEBUGFLAG 40
#define ERRORFLAG 30
#define CRITICALFLAG 20
#define ALLFLAG 10

#define DEBUGLEVEL INFOFLAG

# if DEBUGLEVEL >= DEBUGFLAG
# define DEBUGRET QDebug
# else
# define DEBUGRET QNoDebug
# endif

# if DEBUGLEVEL >= WARNINGFLAG
# define WARNINGRET QDebug
# else
# define WARNINGRET QNoDebug
# endif

# if DEBUGLEVEL >= INFOFLAG
# define INFORET QDebug
# else
# define INFORET QNoDebug
# endif


#include <iostream>
#include <QObject>
#include <map>


//template<typename adaptor>
class Logger: public QObject
{
private:
    static std::map<QString, Logger*> logger_instances;

public:
//    const static int AllFlag = 60;
//    const static int CriticalFlag = 50;
//    const static int ErrorFlag = 40;
//    const static int DebugFlag = 30;
//    const static int WarningFlag = 20;
//    const static int InfoFlag = 10;
private:
    int log_flag;
    Logger(QObject *parent = nullptr);
public:

    static Logger *get_logger(QString logger_name, QObject)


    DEBUGRET debug();
    WARNINGRET warning();
    INFORET info();
};



#endif // LOGGER_H
