//
// Created by mushan
//

#ifndef MY_WEB_SERVER_TIMER_H
#define MY_WEB_SERVER_TIMER_H

#include <queue>
#include <algorithm>
#include <unordered_map>
#include <time.h>
#include <functional>
#include <assert.h>
#include <chrono>
#include <arpa/inet.h>
#include "../logger/logger.h"
#include "../utils/param.h"


typedef std::chrono::high_resolution_clock::time_point Timepoint;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Ms;


struct Event {
    Timepoint expires_time;
    std::function<void()> call_back_func;
    int fd;

    bool operator<(const Event &e) {
        return expires_time < e.expires_time;
    }

};


class Timer {
private:
    std::vector <Event> heap_;
    std::unordered_map<int, int> map_;


public:
    Timer();

    ~Timer() = default;

    void push(int fd, int timeout, const std::function<void()> callback);

    void extend(int fd, int timeout);

    void execute(int fd);

    void tick();

    int nexttick();


private:
    void clean_();

    void swap_(int i, int j);

    void adjust_(int i, int n);

    bool sink_(int i, int n);

    void swim_(int i);

    void modify_(int i, int timeout, const std::function<void()> callback);

    void del_(int i);
};


#endif //MY_WEB_SERVER_TIMER_H
