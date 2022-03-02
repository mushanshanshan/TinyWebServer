//
// Created by mushan
//

#include "logger.h"

const std::vector <std::string> Logger::LEVEL_STR =
        {"[DEBUG]: ", "[INFO]:  ", "[WARN]:  ", "[ERROR]: "};


Logger::Logger()
        : lines_(0), fp_(nullptr) {}


Logger::~Logger() {
    if (writer_thread_ptr_->joinable()) {
        while (!bq_ptr_->is_empty()) {
            bq_ptr_->flush();
        }
        bq_ptr_->close();
        writer_thread_ptr_->join();
    }

    MUTEX_GUARD l(mutex_);
    flush();
    fclose(fp_);
}


Logger *Logger::instancePtr() {
    static Logger instance;
    return &instance;
}


int Logger::getlevel() {
    MUTEX_GUARD l(mutex_);
    return level_;
}

void Logger::init() {
    Logger::instancePtr()->init_();
}

void Logger::init_() {
    level_ = LOGGER_LEVEL;
    path_ = LOGGER_PATH;


    std::unique_ptr <BlockingQueue<std::string>> tempbq(new BlockingQueue<std::string>);
    bq_ptr_ = std::move(tempbq);
    std::unique_ptr <std::thread> tempthr(new std::thread([]() {
        Logger::instancePtr()->aSyncWrite_();
    }));
    writer_thread_ptr_ = std::move(tempthr);

    time_t timer = time(nullptr);
    struct tm *sysTime = localtime(&timer);
    struct tm t = *sysTime;
    date = t.tm_mday;

    char filename[LOGGER_FILE_NAME_MAX_LEN];
    snprintf(filename, LOGGER_FILE_NAME_MAX_LEN - 1, "%s/%04d_%02d_%02d%s",
             path_.c_str(), t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, LOGGER_FILE_SUFFIX);

    fp_ = fopen(filename, "a");
    if (fp_ == nullptr) {
        system("mkdir log");
        fp_ = fopen(filename, "a");
    }
}


void Logger::updateLogFileNameByDate_(struct tm &t) {
    MUTEX_UNIQUE l(mutex_);
    l.unlock();

    char newFile[LOGGER_FILE_NAME_MAX_LEN];
    char tail[36] = {0};
    snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);


    snprintf(newFile, LOGGER_FILE_NAME_MAX_LEN - 72, "%s/%s%s",
             path_.c_str(), tail, LOGGER_FILE_SUFFIX);
    date = t.tm_mday;
    lines_ = 0;


    l.lock();
    flush();
    fclose(fp_);
    fp_ = fopen(newFile, "a");
}

void Logger::write(int level, const char *format, ...) {
    struct timeval now = {0, 0};
    gettimeofday(&now, nullptr);
    time_t tSec = now.tv_sec;
    struct tm *sysTime = localtime(&tSec);
    struct tm t = *sysTime;
    va_list vaList;

    if (date != t.tm_mday) { updateLogFileNameByDate_(t); }


    {
        MUTEX_UNIQUE l(mutex_);
        lines_++;
        int n = snprintf(buffer_.writeStartPtr(), 128, "%d-%02d-%02d %02d:%02d:%02d ",
                         t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                         t.tm_hour, t.tm_min, t.tm_sec);

        buffer_.external_write(n);
        buffer_.put(LEVEL_STR[level]);

        va_start(vaList, format);
        int m = vsnprintf(buffer_.writeStartPtr(), buffer_.write_cap(), format, vaList);
        va_end(vaList);

        buffer_.external_write(m);
        buffer_.put("\n\0", 2);

        if (!bq_ptr_->is_full()) {
            bq_ptr_->push_back(buffer_.toStrAndClean());
        } else {
            fputs(buffer_.readStartPtr(), fp_);
        }
        buffer_.clean();
    }
}

void Logger::flush() {
    bq_ptr_->flush();
}


void Logger::aSyncWrite_() {
    std::string str = "";
    while (bq_ptr_->pop(str)) {
        MUTEX_UNIQUE l(mutex_);
        fputs(str.c_str(), fp_);
    }
}




