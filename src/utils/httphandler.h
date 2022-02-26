//
// Created by mushan on 26/2/22.
//

#ifndef MY_PROJECT_HTTPHANDLER_H
#define MY_PROJECT_HTTPHANDLER_H

#include <sys/types.h>
#include <sys/uio.h>     // readv/writev
#include <arpa/inet.h>   // sockaddr_in
#include <stdlib.h>      // atoi()
#include <errno.h>
#include "httprequest.h"
#include "httpresponse.h"
#include "../logger/logger.h"
#include "../buffer/buffer.h"



class HttpHandler{
private:
    bool isopen_;
    int fd_;
    static const bool ET_;
    struct sockaddr_in addr_;
    struct iovec iov_[2];
    int iovnum_ = 2;

    Buffer inbuffer_;
    Buffer outbuffer_;
    Request request_;
    Response response_;


public:
    HttpHandler():isopen_(false) {};
    ~HttpHandler() {close();};

    void init(int fd, const sockaddr_in addr);
    void close();

    bool read();
    bool write();
    bool process();
    int fd();

    int iovRemain();
    std::string client_ip();
    int client_port();
    bool keepAlive();


private:



};

#endif //MY_PROJECT_HTTPHANDLER_H
