//
// Created by mushan on 26/2/22.
//

#include "server.h"


Server::Server()
    : port_(SERVER_PORT),
      handler_timeout_(SERVER_TIMEOUT),
      uselog_(SERVER_USE_LOGGER),
      loglevel_(SERVER_LOG_LEVEL),
      epoll_timeout_(EPOLL_TIME_OUT),
      timer_(new Timer()),
      epoll_(new Epoll),
      isopen_(false),
      threadPool_(new ThreadPool) {init_();}



Server::~Server() {}


bool Server::init_() {
    if (uselog_) {
        Logger::instancePtr()->init();
    }

    int res;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port_);
    reactorfd_ = socket(AF_INET, SOCK_STREAM, 0);
    assert(reactorfd_ > 2);

    LOG_DEBUG("ReactorFD: %d", reactorfd_);

    if (SERVER_USE_ELEG_DISCONNECT) {
        struct linger optLinger = {1,1};
        res = setsockopt(reactorfd_, SOL_SOCKET, SO_LINGER, &optLinger, sizeof(optLinger));
        assert(res == 0);
    }
    int optval = 1;
    res = setsockopt(reactorfd_, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));
    assert(res == 0);
    res = bind(reactorfd_, (struct sockaddr *)&addr, sizeof(addr));
    assert(res == 0);
    res = listen(reactorfd_, SERVER_REACTOR_BACKLOG);
    res = epoll_->add(reactorfd_, reactor_events_ | EPOLLIN);
    assert(res == 0);
    setNonBlock_(reactorfd_);

    if (EPOLL_TRIGGER_METHOD == "ET") {
        reactor_events_ = EPOLLRDHUP | EPOLLET;
        handlers_events_ = EPOLLONESHOT | EPOLLRDHUP | EPOLLET;
    } else {
        reactor_events_ = EPOLLRDHUP;
        handlers_events_ = EPOLLONESHOT | EPOLLRDHUP;
    }



    LOG_INFO("################### Server Initialization ###################");
    LOG_INFO("Port: &d, Log Level: %d", port_, loglevel_);

    isopen_ = true;
    return true;
}


void Server::setNonBlock_(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}


void Server::handlerClose_(HttpHandler *handler) {
    epoll_->del(handler->fd());
    handler->close();
}

void Server::react_() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    int fd = accept(reactorfd_, (struct sockaddr *)&addr, &len);
    if (fd <= 0) { return;}
    handler_map_[fd].init(fd, addr);
    timer_->push(fd, handler_timeout_, std::bind(&Server::handlerClose_, this, &handler_map_[fd]));
    epoll_->add(fd, handlers_events_ | EPOLLIN);
    setNonBlock_(fd);

    LOG_DEBUG("#New connection: IP: &s", inet_ntoa(addr.sin_addr));
    DEBUGCOUT(inet_ntoa(addr.sin_addr));
}


void Server::readdHandler_(HttpHandler *handler) {
    DEBUGCOUT("readdHandler_")
    if(handler->process()) {
        epoll_->modify(handler->fd(), handlers_events_ | EPOLLOUT);
    } else {
        epoll_->modify(handler->fd(), handlers_events_ | EPOLLIN);
    }
}

void Server::handlerRead_(HttpHandler *handler) {
    DEBUGCOUT("handlerRead_")
    int error = 0;
    do {
        DEBUGCOUT("handlerRead_loop")
        if (handler->read() <= 0) {
            handlerClose_(handler);
            return;
        }
    } while (error == EAGAIN);
    readdHandler_(handler);
}


void Server::handlerWrite_(HttpHandler *handler) {
    int ret = -1;
    int writeErrno = 0;
    ret = handler->write();
    if(handler->iovRemain() == 0) {
        if(handler->keepAlive()) {
            readdHandler_(handler);
            return;
        }
    }
    else if(ret < 0) {
        if(writeErrno == EAGAIN) {
            epoll_->modify(handler->fd(), handlers_events_ | EPOLLOUT);
            return;
        }
    }
    handlerClose_(handler);

}

void Server::callHandlerRead_(HttpHandler *handler) {
    DEBUGCOUT("callHandlerRead_")
    timer_->extend(handler->fd(), handler_timeout_);
    threadPool_->push(std::bind(&Server::handlerRead_, this, handler));
}


void Server::callHandlerWrite_(HttpHandler *handler) {
    DEBUGCOUT("callHandlerWrite_")
    timer_->extend(handler->fd(), handler_timeout_);
    threadPool_->push(std::bind(&Server::handlerWrite_, this, handler));
}


void Server::start() {
    LOG_INFO("################### Server Start ###################");

    DEBUGCOUT("server loop")
    while (isopen_) {
        int events_num = epoll_->wait(epoll_timeout_);
        LOG_DEBUG("Epoll Wait Result: %d", events_num);
        DEBUGCOUT(events_num);
        for (int i=0; i<events_num; i++) {
            int fd = epoll_->getFd(i);
            uint32_t events = epoll_->getEvent(i);
            if (fd == reactorfd_) {
                react_();
            } else if (events & EPOLLIN) {
                callHandlerRead_(&handler_map_[fd]);
            } else if (events & EPOLLOUT) {
                callHandlerWrite_(&handler_map_[fd]);
            } else {
                handlerClose_(&handler_map_[fd]);
            }
        }
    }
}