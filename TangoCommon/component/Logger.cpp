#include "Logger.h"
#include <QDebug>
#include <QMessageLogger>


/*********************************** Singletons ***********************************/

Logger Logger::m_logger_instance;
std::unordered_map<std::string, Logger*> *LoggerManager::m_logger_instances = nullptr;
LoggerManager LoggerManager::logger_manager;


/*********************************** LoggerFlag ***********************************/

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


/************************************* Logger *************************************/

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
    if (!LoggerManager::m_logger_instances->count(logger_name)) {
        alloc_mutex.lock();
        if (!LoggerManager::m_logger_instances->count(logger_name)) {
            (*LoggerManager::m_logger_instances)[logger_name] = new Logger();
        }
        alloc_mutex.unlock();
    }
    return (*LoggerManager::m_logger_instances)[logger_name];
}


/********************************* static methods *********************************/

Logger::OutStream Logger::criticals()
{
    return Logger::m_logger_instance.handler->_critical();
}

Logger::OutStream Logger::errors()
{
    return Logger::m_logger_instance.handler->_error();
}

Logger::OutStream Logger::debugs()
{
    return Logger::m_logger_instance.handler->_debug();
}

Logger::OutStream Logger::warnings()
{
    return Logger::m_logger_instance.handler->_warning();
}

Logger::OutStream Logger::infos()
{
    return Logger::m_logger_instance.handler->_info();
}


/******************************** instance methods ********************************/
Logger::OutStream Logger::critical()
{
    return this->handler->_critical();
}

Logger::OutStream Logger::error()
{
    return this->handler->_error();
}

Logger::OutStream Logger::debug()
{
    return this->handler->_debug();
}

Logger::OutStream Logger::warning()
{
    return this->handler->_warning();
}

Logger::OutStream Logger::info()
{
    return this->handler->_info();
}


/******************************** aborted methods ********************************/

template<typename out_flag>
[[deprecated]] bool Logger::set_mode()
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<out_flag>;
    return this->handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_mode<LoggerFlag::critical_logger_flag>()
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::critical_logger_flag>;
    return this->handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_mode<LoggerFlag::error_logger_flag>()
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::error_logger_flag>;
    return this->handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_mode<LoggerFlag::debug_logger_flag>()
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::debug_logger_flag>;
    return this->handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_mode<LoggerFlag::warning_logger_flag>()
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::warning_logger_flag>;
    return this->handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_mode<LoggerFlag::info_logger_flag>()
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::info_logger_flag>;
    return this->handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_modes<LoggerFlag::logger_flag>()
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::logger_flag>;
    return m_logger_instance.handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_modes<LoggerFlag::critical_logger_flag>()
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::critical_logger_flag>;
    return m_logger_instance.handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_modes<LoggerFlag::error_logger_flag>()
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::error_logger_flag>;
    return m_logger_instance.handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_modes<LoggerFlag::debug_logger_flag>()
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::debug_logger_flag>;
    return m_logger_instance.handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_modes<LoggerFlag::warning_logger_flag>()
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::warning_logger_flag>;
    return m_logger_instance.handler != nullptr;
}

template<>
[[deprecated]] bool Logger::set_modes<LoggerFlag::info_logger_flag>()
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::info_logger_flag>;
    return m_logger_instance.handler != nullptr;
}


template<typename out_flag>
[[deprecated]] bool Logger::set_modes()
{
    qDebug() << "this flag is invalid";
    return false;
}


/******************************** set_mode methods ********************************/

bool Logger::set_mode(LoggerFlag::critical_logger_flag)
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::critical_logger_flag>;
    return this->handler != nullptr;
}

bool Logger::set_mode(LoggerFlag::error_logger_flag)
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::error_logger_flag>;
    return this->handler != nullptr;
}

bool Logger::set_mode(LoggerFlag::debug_logger_flag)
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::debug_logger_flag>;
    return this->handler != nullptr;
}

bool Logger::set_mode(LoggerFlag::warning_logger_flag)
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::warning_logger_flag>;
    return this->handler != nullptr;
}
bool Logger::set_mode(LoggerFlag::info_logger_flag)
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::info_logger_flag>;
    return this->handler != nullptr;
}

bool Logger::set_mode(LoggerFlag::logger_flag)
{
    if (this->handler != nullptr) {
        delete this->handler;
        this->handler = nullptr;
    }
    this->handler = new LoggerHelper<LoggerFlag::logger_flag>;
    return this->handler != nullptr;
}


/******************************** setmodes methods ********************************/

bool Logger::set_modes(LoggerFlag::logger_flag)
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::logger_flag>;
    return m_logger_instance.handler != nullptr;
}

bool Logger::set_modes(LoggerFlag::info_logger_flag)
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::info_logger_flag>;
    return m_logger_instance.handler != nullptr;
}

bool Logger::set_modes(LoggerFlag::warning_logger_flag)
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::warning_logger_flag>;
    return m_logger_instance.handler != nullptr;
}

bool Logger::set_modes(LoggerFlag::debug_logger_flag)
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::debug_logger_flag>;
    return m_logger_instance.handler != nullptr;
}

bool Logger::set_modes(LoggerFlag::error_logger_flag)
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::error_logger_flag>;
    return m_logger_instance.handler != nullptr;
}

bool Logger::set_modes(LoggerFlag::critical_logger_flag)
{
    if (m_logger_instance.handler != nullptr) {
        delete m_logger_instance.handler;
        m_logger_instance.handler = nullptr;
    }
    m_logger_instance.handler = new LoggerHelper<LoggerFlag::critical_logger_flag>;
    return m_logger_instance.handler != nullptr;
}


/******************************** Logger Manager *********************************/

LoggerManager::LoggerManager()
{
    m_logger_instances = nullptr;
    m_logger_instances = new std::unordered_map<std::string, Logger*>;
    if (m_logger_instances == nullptr) {
        throw std::bad_alloc();
    }
    m_logger_instances->clear();
}

LoggerManager::~LoggerManager()
{
    for (auto &logger_instance: *m_logger_instances) {
        if (logger_instance.second != nullptr) {
#ifdef DEBUG
            qDebug() << "logger" << QString::fromStdString(logger_instance.first) << "is destructed";
#endif
            delete logger_instance.second;
            logger_instance.second = nullptr;
        }
    }
    delete m_logger_instances;
    m_logger_instances = nullptr;
}


/******************************** request stream ********************************/

/* 申请假的stream(Not True Type) */
template<typename request_bool_type>
inline LoggerManagable::OutStream LoggerManagable::request_stream() {
    static QString idle_buffer;
    static QDebug idle_dbg(&idle_buffer);
    idle_buffer.clear();
    return idle_dbg;
}

/* 申请真的stream(偏特化True Type) */
template<>
inline LoggerManagable::OutStream LoggerManagable::request_stream<std::true_type>() {
    return qDebug();
}


/****************************** select output or not ******************************/

/* 根据type_traits里记录的值选择对应的stream */
template<typename out_flag>
inline LoggerManagable::OutStream LoggerHelper<out_flag>::_critical()
{
    return request_stream<typename LoggerFlag::type_traitor<out_flag>::enable_critical::type>();
}

template<typename out_flag>
inline LoggerManagable::OutStream LoggerHelper<out_flag>::_error()
{
    return request_stream<typename LoggerFlag::type_traitor<out_flag>::enable_error::type>();
}

template<typename out_flag>
inline LoggerManagable::OutStream LoggerHelper<out_flag>::_debug()
{
    return request_stream<typename LoggerFlag::type_traitor<out_flag>::enable_debug::type>();
}

template<typename out_flag>
inline LoggerManagable::OutStream LoggerHelper<out_flag>::_warning()
{
    return request_stream<typename LoggerFlag::type_traitor<out_flag>::enable_warning::type>();
}


template<typename out_flag>
inline LoggerManagable::OutStream LoggerHelper<out_flag>::_info()
{
    return request_stream<typename LoggerFlag::type_traitor<out_flag>::enable_info::type>();
}


/******************************** void constructor ********************************/

template<typename output_flag>
LoggerHelper<output_flag>::LoggerHelper() {}

template<typename output_flag>
LoggerHelper<output_flag>::~LoggerHelper() {}

LoggerManagable::LoggerManagable() {}

LoggerManagable::~LoggerManagable() {}
