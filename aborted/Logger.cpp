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
    this->handler = new LoggerHelper<LoggerFlag::logger_flag>;
}

Logger::~Logger()
{
    if (handler != nullptr) {
        delete handler;
    }
}

Logger *Logger::get_logger(const std::string &logger_name)
{
    static std::mutex alloc_mutex;
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
    return Logger::m_instance.handler->_critical();
}

Logger::OutStream Logger::critical()
{
    return this->handler->_critical();
}

Logger::OutStream Logger::errors()
{
    return Logger::m_instance.handler->_error();
}

Logger::OutStream Logger::error()
{
    return this->handler->_error();
}

Logger::OutStream Logger::debugs()
{
    return Logger::m_instance.handler->_debug();
}

Logger::OutStream Logger::debug()
{
    return this->handler->_debug();
}
Logger::OutStream Logger::warning()
{
    return this->handler->_warning();
}

Logger::OutStream Logger::warnings()
{
    return Logger::m_instance.handler->_warning();
}

Logger::OutStream Logger::info()
{
    return this->handler->_info();
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
LoggerManagable::OutStream LoggerHelper<out_flag>::_critical()
{
    return LoggerManagable::no_printer;
}

template<>
LoggerManagable::OutStream LoggerHelper<LoggerFlag::critical_logger_flag>::_critical()
{
    return LoggerManagable::printer;
}

template<typename out_flag>
LoggerManagable::OutStream LoggerHelper<out_flag>::_error()
{
    return LoggerManagable::no_printer;
}

template<>
LoggerManagable::OutStream LoggerHelper<LoggerFlag::error_logger_flag>::_error()
{
    return LoggerManagable::printer;
}

template<typename out_flag>
LoggerManagable::OutStream LoggerHelper<out_flag>::_debug()
{
    return LoggerManagable::no_printer;
}

template<>
LoggerManagable::OutStream LoggerHelper<LoggerFlag::debug_logger_flag>::_debug()
{
    return LoggerManagable::printer;
}

template<typename out_flag>
LoggerManagable::OutStream LoggerHelper<out_flag>::_warning()
{
    return LoggerManagable::no_printer;
}

template<>
LoggerManagable::OutStream LoggerHelper<LoggerFlag::warning_logger_flag>::_warning()
{
    return LoggerManagable::printer;
}

template<typename out_flag>
LoggerManagable::OutStream LoggerHelper<out_flag>::_info()
{
    return LoggerManagable::no_printer;
}

template<>
LoggerManagable::OutStream LoggerHelper<LoggerFlag::info_logger_flag>::_info()
{
    return LoggerManagable::printer;
}

template<typename output_flag>
LoggerHelper<output_flag>::LoggerHelper()
{

}

template<typename output_flag>
LoggerHelper<output_flag>::~LoggerHelper()
{

}


LoggerManagable::LoggerManagable() {}
LoggerManagable::~LoggerManagable() {}

QNNNNNNoDebug::QNNNNNNoDebug(): QDebug(qDebug())
{

}

QNNNNNNoDebug::QNNNNNNoDebug(const QNNNNNNoDebug &): QDebug(qDebug())
{

}

QNNNNNNoDebug::QNNNNNNoDebug(const QNNNNNNoDebug &&): QDebug(qDebug())
{

}

QNNNNNNoDebug::~QNNNNNNoDebug()
{
}

QNNNNNNoDebug QNNNNNNoDebug::operator()()
{
    static QNNNNNNoDebug idle_printer;
    return idle_printer;
}

template<typename input_type>
QNNNNNNoDebug &QNNNNNNoDebug::operator<<(input_type)
{
    return *this;
}
