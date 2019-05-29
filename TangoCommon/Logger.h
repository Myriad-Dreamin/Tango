#ifndef LOGGGGER_H
#define LOGGGGER_H


#include <QString>
#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <type_traits>
#include <QDebug>

namespace LoggerFlag {
    struct logger_flag                                       {};
    struct critical_logger_flag : public logger_flag         {};
    struct error_logger_flag    : public critical_logger_flag{};
    struct debug_logger_flag    : public error_logger_flag   {};
    struct warning_logger_flag  : public debug_logger_flag   {};
    struct info_logger_flag     : public warning_logger_flag {};

    template<typename m_flag> struct type_traitor;
}

class LoggerQStream{
public:
    inline QDebug stream() {return qDebug();}
};

class LoggerQDebugStream: public LoggerQStream
{
public:
    LoggerQDebugStream() {}
    inline QDebug stream() {return qDebug();}
};


class QNNNNNNoDebug: public QDebug
{
public:
    QNNNNNNoDebug();
    QNNNNNNoDebug(const QNNNNNNoDebug &x);
    QNNNNNNoDebug(const QNNNNNNoDebug &&x);
    ~QNNNNNNoDebug();
    QNNNNNNoDebug operator() ();
    template<typename input_type>
    inline QNNNNNNoDebug &operator<<(input_type ele);
};


class LoggerQNoDebugStream: public LoggerQStream
{
public:
    LoggerQNoDebugStream() {}
    inline QDebug stream() {return QNNNNNNoDebug();}
};

class LoggerManagable
{
protected:
    typedef LoggerQStream OutStream;
    static LoggerQDebugStream printer;
    static LoggerQNoDebugStream no_printer;
public:
    LoggerManagable();
    virtual ~LoggerManagable() = 0;

    virtual OutStream _critical() = 0;
    virtual OutStream _error() = 0;
    virtual OutStream _debug() = 0;
    virtual OutStream _warning() = 0;
    virtual OutStream _info() = 0;
};

class Logger
{
protected:
    typedef LoggerQStream OutStream;
public:

    static Logger *get_logger(const std::string &logger_name);

    static OutStream criticals();
    static OutStream errors();
    static OutStream debugs();
    static OutStream warnings();
    static OutStream infos();

    bool set_mode(LoggerFlag::logger_flag flag);

    OutStream critical();
    OutStream error();
    OutStream debug();
    OutStream warning();
    OutStream info();

public:
    class LoggerManager{
    public:
        LoggerManager();
        ~LoggerManager();
    };
protected:
    Logger();
    virtual ~Logger();
private:
    template<typename out_flag=LoggerFlag::logger_flag>
    Logger(out_flag);
    LoggerManagable *handler;
    static std::unordered_map<std::string, Logger*> logger_instances;
    static Logger m_instance;
    static LoggerManager logger_destructor;
};

template <typename output_flag>
class LoggerHelper: public LoggerManagable
{
public:
    LoggerHelper();
    ~LoggerHelper();

    OutStream _critical();
    OutStream _error();
    OutStream _debug();
    OutStream _warning();
    OutStream _info();
};

std::unordered_map<std::string, Logger*> Logger::logger_instances;
Logger Logger::m_instance;
Logger::LoggerManager Logger::logger_destructor;
LoggerQDebugStream LoggerManagable::printer;
LoggerQNoDebugStream LoggerManagable::no_printer;



#endif // LOGGGGER_H
