#ifndef LOGGER_H
#define LOGGER_H


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

class LoggerQNoDebugStream: public LoggerQStream
{
public:
    LoggerQNoDebugStream() {}
    inline QDebug stream() {return qWarning();}
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
    static LoggerQDebugStream printer;
    static LoggerQNoDebugStream no_printer;
    Logger();
    virtual ~Logger();
private:
    template<typename out_flag=LoggerFlag::logger_flag>
    Logger(out_flag);
    Logger *handler;
    static std::mutex alloc_mutex;
    static std::unordered_map<std::string, Logger*> logger_instances;
    static Logger m_instance;
    static LoggerManager logger_destructor;

    virtual OutStream _critical();
    virtual OutStream _error();
    virtual OutStream _debug();
    virtual OutStream _warning();
    virtual OutStream _info();
};

template <typename output_flag>
class LoggerHelper: public Logger
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

std::mutex Logger::alloc_mutex;
std::unordered_map<std::string, Logger*> Logger::logger_instances;
Logger Logger::m_instance;
Logger::LoggerManager Logger::logger_destructor;
LoggerQDebugStream Logger::printer;
LoggerQNoDebugStream Logger::no_printer;


#endif // LOGGER_H
