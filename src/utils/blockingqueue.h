//
// Created by mushan on 24/2/22.
//

#ifndef MY_PROJECT_BLOCKINGQUEUE_H
#define MY_PROJECT_BLOCKINGQUEUE_H


#include <mutex>
#include <condition_variable>
#include <deque>
#include <sys/time.h>
#include "param.h"

typedef std::lock_guard<std::mutex> MUTEX_GUARD;
typedef std::unique_lock<std::mutex> MUTEX_UNIQUE;


template <class T>
class BlockingQueue{
private:
    std::mutex mutex_;
    std::condition_variable conin_;
    std::condition_variable conout_;
    std::deque<T> dq_;
    bool is_open_;
    size_t capacity_ = BLOCKING_QUEUE_LENGTH;

public:
    BlockingQueue();
    ~BlockingQueue();

    BlockingQueue(const BlockingQueue& b) = delete;

    void push_back(const T& t);
    void push_front(const T& t);

    bool is_full();
    bool is_empty();
    size_t size();
    size_t capacity();
    T front();
    T back();
    bool pop(T &t);
    bool pop();
    void flush();

    void close();



};

template <class T>
BlockingQueue<T>::BlockingQueue()
                : mutex_(),
                  conin_(),
                  conout_(),
                  is_open_(true)
                  {
                      capacity_ = BLOCKING_QUEUE_LENGTH;
                  }



template <class T>
BlockingQueue<T>::~BlockingQueue() {
    MUTEX_GUARD l(mutex_);
    dq_.clear();
    is_open_ = false;
    conout_.notify_all();
    conin_.notify_all();
}



template <class T>
void BlockingQueue<T>::push_back(const T &t) {

    MUTEX_UNIQUE l(mutex_);
    while (dq_.size() >= capacity_) {
        conin_.wait(l);
        if (!is_open_) return;
    }
    dq_.push_back(t);
    conout_.notify_one();
}

template <class T>
void BlockingQueue<T>::push_front(const T &t) {

    MUTEX_UNIQUE l(mutex_);
    while (dq_.size() >= capacity_) {
        conin_.wait(l);
        if (!is_open_) return;
    }
    dq_.push_front(t);
    conout_.notify_one();
}

template <class T>
bool BlockingQueue<T>::is_full() {
    MUTEX_GUARD l(mutex_);
    return dq_.size() >= capacity_;
}

template <class T>
bool BlockingQueue<T>::is_empty() {
    MUTEX_GUARD l(mutex_);
    return dq_.empty();
}

template <class T>
size_t BlockingQueue<T>::capacity() {
    MUTEX_GUARD l(mutex_);
    return capacity_;
}

template <class T>
size_t BlockingQueue<T>::size() {
    MUTEX_GUARD l(mutex_);
    return dq_.size();
}

template <class T>
T BlockingQueue<T>::front() {
    MUTEX_GUARD l(mutex_);
    return dq_.front();
}


template <class T>
T BlockingQueue<T>::back() {
    MUTEX_GUARD l(mutex_);
    return dq_.back();
}

template <class T>
bool BlockingQueue<T>::pop(T &t) {
    MUTEX_UNIQUE l(mutex_);
    while(dq_.empty()) {
        conout_.wait(l);
        if (!is_open_) return false;
    }
    t = dq_.front();
    dq_.pop_front();
    return true;
}

template <class T>
bool BlockingQueue<T>::pop() {
    MUTEX_UNIQUE l(mutex_);
    while(dq_.empty()) {
        conout_.wait(l);
        if (!is_open_) return false;
    }
    dq_.pop_front();
    return true;
}

template <class T>
void BlockingQueue<T>::flush() {
    conout_.notify_all();
}


template <class T>
void BlockingQueue<T>::close() {
    {
        MUTEX_GUARD l(mutex_);
        dq_.clear();
        is_open_ = false;
    }
    conout_.notify_all();
    conin_.notify_all();
}


#endif //MY_PROJECT_BLOCKINGQUEUE_H
