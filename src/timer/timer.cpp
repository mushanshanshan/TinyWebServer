//
// Created by mushan
//

#include "timer.h"

Timer::Timer() {
    heap_.reserve(TIMER_HEAP_SIZE);
}


void Timer::swap_(int i, int j) {
    std::swap(heap_[i], heap_[j]);
    map_[heap_[i].fd] = i;
    map_[heap_[j].fd] = j;
}

void Timer::swim_(int i) {
    int parent = 0;
    for (; i > 0; i = parent) {
        parent = (i - 1) / 2;
        if (heap_[i] < heap_[parent]) {
            swap_(i, parent);
        } else {
            break;
        }
    }
}


bool Timer::sink_(int i, int n) {
    int i_ = i, child;
    for (; i * 2 + 1 < n; i = child) {
        child = i * 2 + 1;
        if (child < n-1 && heap_[child+1] < heap_[child]) {
            child++;
        }
        if (heap_[child] < heap_[i]) {
            swap_(child, i);
        } else {
            break;
        }
    }
    return i > i_;
}


void Timer::adjust_(int i, int n) {
    if (!sink_(i, n)) swim_(i);
}


void Timer::modify_(int fd, int timeout, const std::function<void()> callback) {
    int idx = map_[fd];
    heap_[idx].call_back_func = callback;
    heap_[idx].expires_time = Clock::now() + Ms(timeout);
    adjust_(idx, heap_.size());
}



void Timer::push(int fd, int timeout, const std::function<void()> callback) {
    if (map_.find(fd) != map_.end()) {
        modify_(fd, timeout, callback);
    } else {
        heap_.push_back({Clock::now() + Ms(timeout), callback, fd});
        map_[fd] = heap_.size() - 1;
        swim_(map_[fd]);
    }
}

void Timer::extend(int fd, int timeout) {
    heap_[map_[fd]].expires_time = Clock::now() + Ms(timeout);
    sink_(map_[fd], heap_.size());
}

void Timer::del_(int i) {
    int j = heap_.size() - 1;
    if (i < j) {
        swap_(i, j);
        adjust_(i, j);
    }
    map_.erase(heap_.back().fd);
    heap_.pop_back();
}

void Timer::execute(int fd) {
    heap_[map_[fd]].call_back_func();
    del_(fd);
}

void Timer::tick() {
    while(!heap_.empty()) {
        Event e = heap_.front();
        if (std::chrono::duration_cast<Ms>(e.expires_time - Clock::now()).count() < 0) {
            e.call_back_func();
            del_(0);
        } else break;
    }
}

int Timer::nexttick() {
    int res = -1;
    if(!heap_.empty()) {
        res = std::chrono::duration_cast<Ms>(heap_.front().expires_time - Clock::now()).count();
        if(res < 0) { res = 0; }
    }
    return res;
}
