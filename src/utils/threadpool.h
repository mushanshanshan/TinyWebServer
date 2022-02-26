//
// Created by mushan on 25/2/22.
//

#ifndef MY_PROJECT_THREADPOOL_H
#define MY_PROJECT_THREADPOOL_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "../utils/param.h"

typedef std::lock_guard<std::mutex> MUTEX_GUARD;
typedef std::unique_lock<std::mutex> MUTEX_UNIQUE;


class ThreadPool {
private:
    struct Pool {
        std::mutex mutex;
        std::condition_variable cond;
        bool isclosed;
        std::queue<std::function<void()>> tasks;
    };

    std::shared_ptr<Pool> pool_;
public:

    explicit ThreadPool()
            :pool_(std::make_shared<Pool>()) {
        for (int i=0; i < THREAD_POOL_THREAD_NUM; i++) {
            std::thread([pool = pool_] {
                std::unique_lock<std::mutex> lk(pool->mutex);
                for (;;) {
                    if (!pool->tasks.empty()) {
                        auto func = std::move(pool->tasks.front());
                        pool->tasks.pop();
                        lk.unlock();
                        func();
                        lk.lock();
                    } else if (pool->isclosed) {
                        break;
                    } else {
                        pool->cond.wait(lk);
                    }
                }
            }).detach();
        }
    }

    ThreadPool(ThreadPool&&) = default;
    ~ThreadPool() {
        MUTEX_GUARD l(pool_->mutex);
        pool_->isclosed = true;
        pool_->cond.notify_all();
    }

    template <class F>
    void push(F&& task) {
        {
            std::lock_guard<std::mutex> lk(pool_->mutex);
            pool_->tasks.emplace(std::forward<F>(task));
        }
        pool_->cond.notify_one();
    }


};

#endif //MY_PROJECT_THREADPOOL_H
