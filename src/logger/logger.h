//
// Created by mushan
//

#ifndef MY_WEB_SERVER_LOGGER_H
#define MY_WEB_SERVER_LOGGER_H

#include <mutex>
#include <string>
#include <sys/time.h>
#include <stdarg.h>
#include <assert.h>
#include <thread>
#include <string.h>
#include "../buffer/buffer.h"
#include "../utils/blockingqueue.h"

class Logger {
private:
    FILE *fp_;
    std::mutex mutex_;
    Buffer buffer_;
    std::unique_ptr <BlockingQueue<std::string>> bq_ptr_;
    std::unique_ptr <std::thread> writer_thread_ptr_;


    int level_;
    size_t lines_;
    int date;
    std::string path_;
    const static std::vector <std::string> LEVEL_STR;


public:

    static Logger &instanceRef();

    static Logger *instancePtr();

    int getlevel();


    void write(int level, const char *format, ...);

    void flush();

    void init();

private:
    Logger();

    ~Logger();

    void init_();

    void aSyncWrite_();


    void updateLogFileNameByDate_(struct tm &t);


};


#define LOG_BASE(level, format, ...) \
    if (SERVER_USE_LOGGER) {         \
            do {\
        Logger* log = Logger::instancePtr();\
        if (log->getlevel() <= level) {\
            log->write(level, format, ##__VA_ARGS__); \
            log->flush();\
        }\
    } while(0);                         \
                                     \
                                     \
    }                                \



#define LOG_DEBUG(format, ...) do {LOG_BASE(0, format, ##__VA_ARGS__)} while(0);
#define LOG_INFO(format, ...) do {LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(3, format, ##__VA_ARGS__)} while(0);


#endif //MY_WEB_SERVER_LOGGER_H
