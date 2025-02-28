#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdarg.h>
#include <cstdio>
#include <iostream>
#include <cstring>

#define LOG_INIT(m) instance = Logger::get_instance(m)

#define LOG_DEBUG(format, ...) Logger::get_instance()->log(Logger::LogLevel::Debug, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) Logger::get_instance()->log(Logger::LogLevel::Info, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) Logger::get_instance()->log(Logger::LogLevel::Warning, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Logger::get_instance()->log(Logger::LogLevel::Error, format, ##__VA_ARGS__)

class Logger {
    public:
        enum LogLevel {
            Debug = 0,
            Info = 1,
            Warning = 2,
            Error = 3
        };

    public:
        static Logger* get_instance(const char* module = nullptr);
        void log(LogLevel level, const char* format, ...);

    protected:
        void do_log(LogLevel level, const char* format, va_list args);

        Logger(const char* module);
        ~Logger();

    protected:
        static Logger* instance;
        LogLevel m_level = LogLevel::Debug;
        const char* m_module = nullptr;
};


#endif
