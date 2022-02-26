//
// Created by mushan on 26/2/22.
//

#ifndef MY_PROJECT_SERVER_H
#define MY_PROJECT_SERVER_H


#include <unordered_map>
#include <fcntl.h>
#include <cassert>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../utils/param.h"
#include "../utils/epoll.h"
#include "../utils/httphandler.h"
#include "../utils/threadpool.h"
#include "../timer/timer.h"
#include "../logger/logger.h"



class Server {
private:
    std::unique_ptr<Timer> timer_;
    std::unique_ptr<Epoll> epoll_;
    std::unique_ptr<ThreadPool> threadPool_;
    std::unordered_map<int, HttpHandler> handler_map_;

    int port_;
    int handler_timeout_;
    bool uselog_;
    bool isopen_;
    int loglevel_;
    int epoll_timeout_;

    uint32_t reactor_events_;
    uint32_t handlers_events_;
    int reactorfd_;

public:
    Server();
    ~Server();
    void start();



private:
    bool init_();
    void setNonBlock_(int fd);
    void react_();

    void callHandlerRead_(HttpHandler* handler);
    void callHandlerWrite_(HttpHandler* handler);
    void readdHandler_(HttpHandler* handler);

    void handlerRead_(HttpHandler* handler);
    void handlerWrite_(HttpHandler* handler);
    void handlerClose_(HttpHandler* handler);



};



#endif //MY_PROJECT_SERVER_H
