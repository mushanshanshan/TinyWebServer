//
// Created by mushan
//

#include "httprequest.h"

void Request::clear() {
    method_.clear();
    path_.clear();
    version_.clear();
    header_.clear();
    keepalive_ = false;
    step_ = REQUEST_LINE;
}



bool Request::parseRequestLine_(std::string line) {
    std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
    std::smatch subMatch;
    if(regex_match(line, subMatch, patten)) {
        method_ = subMatch[1];
        path_ = subMatch[2];
        version_ = subMatch[3];
        step_ = HEADERS;

        if(path_ == "/") path_ = "/index.html";
        return true;
    }
    return false;
}

bool Request::parseRequestHead_(std::string line) {
    std::regex patten("^([^:]*): ?(.*)$");
    std::smatch subMatch;
    if (regex_match(line, subMatch, patten)) {
        header_[subMatch[1]] = subMatch[2];
        if (!keepalive_ && subMatch[1] == "Connection" && subMatch[2] == "keep-alive") {
            keepalive_ = true;
        }
    } else step_ = BODY;
    return true;
}


bool Request::parse(Buffer &buff) {
    if (buff.read_cap() <= 0) return false;

    char CRLF[] = "\r\n";
    step_ = REQUEST_LINE;
    while (buff.read_cap() && step_ != FINISH) {
        char* end = std::search(buff.readStartPtr(), buff.writeStartPtr(), CRLF, CRLF+2);
        std::string line(buff.readStartPtr(), end);
        if (step_ == REQUEST_LINE) {
            if (!parseRequestLine_(line)) return false;
        } else if (step_ == HEADERS) {
            parseRequestHead_(line);
        } else if (step_ == BODY) {
            step_ = FINISH;
        }
        buff.clean(end + 2);
        LOG_DEBUG("[%s], [%s], [%s]", method_.c_str(), path_.c_str(), version_.c_str());
    }
    return true;
}

bool Request::isKeepAlive() {
    return keepalive_;
}

std::string Request::method() {
    return method_;
}

std::string Request::version() {
    return version_;
}

std::string Request::path() {
    return path_;
}

const std::unordered_map<std::string, std::string> Request::head() {
    return header_;
}