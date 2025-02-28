#include "logger.h"

using namespace std;


Logger* Logger::instance = nullptr;

Logger* Logger::get_instance(const char* module) {
    if (instance == nullptr) {
        instance = new Logger(module);
    }
    return instance;
}

void Logger::log(LogLevel level, const char* format, ...) {
    va_list args;
    // int len = vprintf(format, args);
    char* buffer = new char[1024];
    va_start(args, format);
    // Process the arguments here
    vsprintf(buffer, format, args);
    va_end(args);

    do_log(level, buffer, args);
    if (buffer) {
        delete[] buffer;
    }
}

void Logger::do_log(LogLevel level, const char* buffer, va_list args) {
    if (level >= m_level && level < 4) {
        static const char* level_str[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt) - 1] = '\0';
        fprintf(stdout, "[%s][%s][%s]: %s\n", dt, m_module, level_str[level], buffer);
    }
}

Logger::Logger(const char* module) : m_module(module) {
}

Logger::~Logger() {}
