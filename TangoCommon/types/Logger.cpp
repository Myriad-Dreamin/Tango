#include "Logger.h"
#include <QDebug>
#include <QMessageLogger>


namespace LoggerFlag {

    template<typename m_flag>
    struct type_traitor {
        typedef std::true_type enable_info;
        typedef std::true_type enable_warning;
        typedef std::true_type enable_debug;
        typedef std::true_type enable_error;
        typedef std::true_type enable_critical;
    };

    template<>
    struct type_traitor<critical_logger_flag> {
        typedef logger_flag _ww;
        typedef std::false_type enable_info;
        typedef std::false_type enable_warning;
        typedef std::false_type enable_debug;
        typedef std::false_type enable_error;
        typedef std::true_type enable_critical;
    };

    template<>
    struct type_traitor<error_logger_flag> {
        typedef logger_flag _ww;
        typedef std::false_type enable_info;
        typedef std::false_type enable_warning;
        typedef std::false_type enable_debug;
        typedef std::true_type enable_error;
        typedef std::true_type enable_critical;
    };

    template<>
    struct type_traitor<debug_logger_flag> {
        typedef logger_flag _ww;
        typedef std::false_type enable_info;
        typedef std::false_type enable_warning;
        typedef std::true_type enable_debug;
        typedef std::true_type enable_error;
        typedef std::true_type enable_critical;
    };

    template<>
    struct type_traitor<warning_logger_flag> {
        typedef logger_flag _ww;
        typedef std::false_type enable_info;
        typedef std::true_type enable_warning;
        typedef std::true_type enable_debug;
        typedef std::true_type enable_error;
        typedef std::true_type enable_critical;
    };

    template<>
    struct type_traitor<info_logger_flag> {
        typedef logger_flag _ww;
        typedef std::false_type enable_info;
        typedef std::true_type enable_warning;
        typedef std::true_type enable_debug;
        typedef std::true_type enable_error;
        typedef std::true_type enable_critical;
    };
}


Logger::Logger()
{
    handler = new LoggerHelper<LoggerFlag::logger_flag>;
}

Logger::~Logger()
{
    if (handler != nullptr) {
        delete handler;
    }
}

template<typename out_flag>
Logger::Logger(out_flag)
{
    handler = new LoggerHelper<out_flag>;
}


Logger *Logger::get_logger(const std::string &logger_name)
{
    if (!logger_instances.count(logger_name)) {
        alloc_mutex.lock();
        if (!logger_instances.count(logger_name)) {
            return logger_instances[logger_name] = new Logger();
        }
        alloc_mutex.unlock();
    }
    return logger_instances[logger_name];
}

Logger::OutStream Logger::criticals()
{
    return Logger::m_instance._critical();
}

Logger::OutStream Logger::critical()
{
    return this->handler->_critical();
}

Logger::OutStream Logger::_critical()
{
    return Logger::m_instance._critical();
}

Logger::OutStream Logger::errors()
{
    return Logger::m_instance._error();
}

Logger::OutStream Logger::error()
{
    return this->handler->_error();
}

Logger::OutStream Logger::_error()
{
    return Logger::m_instance._error();
}

Logger::OutStream Logger::debugs()
{
    return Logger::m_instance._debug();
}

Logger::OutStream Logger::debug()
{
    return this->handler->_debug();
}

Logger::OutStream Logger::_debug()
{
    return Logger::m_instance._debug();
}

Logger::OutStream Logger::warning()
{
    return this->handler->_warning();
}

Logger::OutStream Logger::_warning()
{
    return Logger::m_instance._warning();
}

Logger::OutStream Logger::warnings()
{
    return Logger::m_instance._warning();
}

Logger::OutStream Logger::info()
{
    return this->handler->_info();
}

Logger::OutStream Logger::_info()
{
    return Logger::m_instance._info();
}

bool Logger::set_mode(LoggerFlag::logger_flag flag)
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<decltype(flag)>;
    return this->handler != nullptr;
}

Logger::LoggerManager::LoggerManager()
{
    logger_instances.clear();
}

Logger::LoggerManager::~LoggerManager()
{
    for (auto &logger_instance: Logger::logger_instances) {
        if (logger_instance.second != nullptr) {
#ifdef DEBUG
            qDebug() << "logger" << QString::fromStdString(logger_instance.first) << "is destructed";
#endif
            delete logger_instance.second;
            logger_instance.second = nullptr;
        }
    }
}

template<typename out_flag>
Logger::OutStream LoggerHelper<out_flag>::_critical()
{
    return Logger::no_printer;
}

template<>
Logger::OutStream LoggerHelper<LoggerFlag::critical_logger_flag>::_critical()
{
    return Logger::printer;
}

template<typename out_flag>
Logger::OutStream LoggerHelper<out_flag>::_error()
{
    return Logger::no_printer;
}

template<>
Logger::OutStream LoggerHelper<LoggerFlag::error_logger_flag>::_error()
{
    return Logger::printer;
}

template<typename out_flag>
Logger::OutStream LoggerHelper<out_flag>::_debug()
{
    return Logger::no_printer;
}

template<>
Logger::OutStream LoggerHelper<LoggerFlag::debug_logger_flag>::_debug()
{
    return Logger::printer;
}

template<typename out_flag>
Logger::OutStream LoggerHelper<out_flag>::_warning()
{
    return Logger::no_printer;
}

template<>
Logger::OutStream LoggerHelper<LoggerFlag::warning_logger_flag>::_warning()
{
    return Logger::printer;
}

template<typename out_flag>
Logger::OutStream LoggerHelper<out_flag>::_info()
{
    return Logger::no_printer;
}

template<>
Logger::OutStream LoggerHelper<LoggerFlag::info_logger_flag>::_info()
{
    return Logger::printer;
}

template<typename output_flag>
LoggerHelper<output_flag>::LoggerHelper()
{

}

template<typename output_flag>
LoggerHelper<output_flag>::~LoggerHelper()
{

}
