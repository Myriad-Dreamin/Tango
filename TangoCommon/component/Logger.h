#ifndef LOGGGGER_H
#define LOGGGGER_H


#include <QString>
#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <type_traits>
#include <QDebug>


/* Logger级别 */
namespace LoggerFlag {
    struct logger_flag                                       {};
    struct critical_logger_flag : public logger_flag         {};
    struct error_logger_flag    : public critical_logger_flag{};
    struct debug_logger_flag    : public error_logger_flag   {};
    struct warning_logger_flag  : public debug_logger_flag   {};
    struct info_logger_flag     : public warning_logger_flag {};

    template<typename m_flag> struct type_traitor;
}

/* Logger指针 */
class LoggerManagable
{
protected:
    typedef QDebug OutStream;
public:
    LoggerManagable();
    virtual ~LoggerManagable() = 0;

    virtual OutStream _critical() = 0;
    virtual OutStream _error() = 0;
    virtual OutStream _debug() = 0;
    virtual OutStream _warning() = 0;
    virtual OutStream _info() = 0;
    template<typename request_bool_type>
    inline LoggerManagable::OutStream request_stream();
};

/* Logger单例 */
class Logger
{
    static Logger m_logger_instance;
    friend class LoggerManager;
protected:
    typedef QDebug OutStream;
public:

    static Logger *get_logger(const std::string &logger_name);

    static OutStream criticals();
    static OutStream errors();
    static OutStream debugs();
    static OutStream warnings();
    static OutStream infos();

    template<typename out_flag>
    [[deprecated]] bool set_mode();
    bool set_mode(LoggerFlag::logger_flag);
    bool set_mode(LoggerFlag::critical_logger_flag);
    bool set_mode(LoggerFlag::error_logger_flag);
    bool set_mode(LoggerFlag::debug_logger_flag);
    bool set_mode(LoggerFlag::warning_logger_flag);
    bool set_mode(LoggerFlag::info_logger_flag);

    template<typename out_flag>
    [[deprecated]] static bool set_modes();
    static bool set_modes(LoggerFlag::logger_flag);
    static bool set_modes(LoggerFlag::critical_logger_flag);
    static bool set_modes(LoggerFlag::error_logger_flag);
    static bool set_modes(LoggerFlag::debug_logger_flag);
    static bool set_modes(LoggerFlag::warning_logger_flag);
    static bool set_modes(LoggerFlag::info_logger_flag);

    OutStream critical();
    OutStream error();
    OutStream debug();
    OutStream warning();
    OutStream info();

public:
protected:
    Logger();
    virtual ~Logger();
private:
    template<typename out_flag=LoggerFlag::logger_flag>
    Logger(out_flag);
    LoggerManagable *handler;
};

/* Logger单例管理器 */
class LoggerManager{
    friend class Logger;
    static std::unordered_map<std::string, Logger*> *m_logger_instances;
    static LoggerManager logger_manager;
public:
    LoggerManager();
    ~LoggerManager();
};


/* Logger指针 */
template <typename output_flag>
class LoggerHelper: public LoggerManagable
{
public:
    LoggerHelper();
    ~LoggerHelper();

    inline OutStream _critical();
    inline OutStream _error();
    inline OutStream _debug();
    inline OutStream _warning();
    inline OutStream _info();
};


#endif // LOGGGGER_H
