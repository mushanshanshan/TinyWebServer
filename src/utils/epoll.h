//
// Created by mushan
//

#ifndef MY_PROJECT_EPOLL_H
#define MY_PROJECT_EPOLL_H

#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <vector>
#include <errno.h>
#include "../utils/param.h"

class Epoll {
private:
    int epoll_fd_;
    std::vector<struct epoll_event> events_;


public:
    Epoll()
            :epoll_fd_(epoll_create(EPOLL_LENGTH)),
             events_(EPOLL_MAX_EVENTS_LENGTH) {}

    ~Epoll() {
        close(epoll_fd_);
    }


    bool add(int fd, uint32_t events) {
        epoll_event ev;
        ev.data.fd = fd;
        ev.events = events;
        return epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
    }

    bool modify(int fd, uint32_t events) {
        epoll_event ev;
        ev.data.fd = fd;
        ev.events = events;
        return epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev);
    }

    bool del(int fd) {
        epoll_event ev;
        return epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &ev);
    }

    int wait(int timeout) {
        return epoll_wait(epoll_fd_, &events_[0], EPOLL_MAX_EVENTS_LENGTH, timeout);
    }

    int getFd(int i) const {
        return events_[i].data.fd;
    }

    int getEvent(int i) const {
        return events_[i].events;
    }


private:
};


#endif //MY_PROJECT_EPOLL_H
