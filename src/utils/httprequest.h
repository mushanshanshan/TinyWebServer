//
// Created by mushan
//

#ifndef MYWEBSERVER_HTTP_REQUEST_PASER_H
#define MYWEBSERVER_HTTP_REQUEST_PASER_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <errno.h>

#include "../buffer/buffer.h"
#include "../logger/logger.h"


class Request {
private:
    std::string method_, path_, version_;
    std::unordered_map<std::string, std::string> header_;
    bool keepalive_ = false;

    enum PARSE_STEP {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };
    PARSE_STEP step_;

public:
    Request() = default;

    ~Request() = default;

    void clear();
    bool parse(Buffer& buff);

    bool isKeepAlive();
    std::string path();
    std::string method();
    std::string version();
    const std::unordered_map<std::string, std::string> head();


private:
    bool parseRequestLine_(std::string line);
    bool parseRequestHead_(std::string line);

};



#endif //MYWEBSERVER_HTTP_REQUEST_PASER_H
