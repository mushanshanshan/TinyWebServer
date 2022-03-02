//
// Created by mushan
//

#include "httphandler.h"

const bool HttpHandler::ET_ = EPOLL_TRIGGER_METHOD == "ET" ? true : false;

void HttpHandler::init(int fd, const sockaddr_in addr) {
    fd_ = fd;
    addr_ = addr;
    inbuffer_.clean();
    outbuffer_.clean();
    isopen_ = true;
    LOG_INFO("Client[%d](%s:%d) connected", fd_, inet_ntoa(addr_.sin_addr), addr_.sin_port)
}


void HttpHandler::close() {
    response_.munmap();
    ::close(fd_);
    isopen_ = false;
    LOG_INFO("Client[%d](%s:%d) disconnected", fd_, inet_ntoa(addr_.sin_addr), addr_.sin_port)
}


std::string HttpHandler::client_ip() {
    return std::string(inet_ntoa(addr_.sin_addr));
}

int HttpHandler::client_port() {
    return addr_.sin_port;
}

int HttpHandler::iovRemain() {
    return iov_[0].iov_len + iov_[1].iov_len;
}


int HttpHandler::fd() {
    return fd_;
}


bool HttpHandler::read() {
    int error = 0;
    ssize_t len = -1;
    do {
        len = inbuffer_.read(fd_, &error);
        if (error != 0 && error != EAGAIN) {
            LOG_ERROR("Client[%d](%s:%d) error: %s", fd_, inet_ntoa(addr_.sin_addr), addr_.sin_port, strerror(error));
            return false;
        }
    } while (len > 0 && ET_);
    return true;
}

bool HttpHandler::write() {
    ssize_t len = -1;
    do {
        len = writev(fd_, iov_, iovnum_);
        if (len <= 0 && errno != EAGAIN) {
            LOG_ERROR("Client[%d](%s:%d) error: %s", fd_, inet_ntoa(addr_.sin_addr), addr_.sin_port, strerror(errno));
            return false;
        }
        if (iov_[0].iov_len == 0 && iov_[1].iov_len == 0) {return true;}
        else if(static_cast<size_t>(len) > iov_[0].iov_len) {
            iov_[1].iov_base = (uint8_t*) iov_[1].iov_base + (len - iov_[0].iov_len);
            iov_[1].iov_len -= (len - iov_[0].iov_len);
            if(iov_[0].iov_len) {
                outbuffer_.clean();
                iov_[0].iov_len = 0;
            }
        }
        else {
            iov_[0].iov_base = (uint8_t*)iov_[0].iov_base + len;
            iov_[0].iov_len -= len;
            outbuffer_.clean(len);
        }
    } while (ET_);
    return true;
}

bool HttpHandler::process() {
    request_.clear();
    if(inbuffer_.read_cap() <= 0) {
        return false;
    }
    else if(request_.parse(inbuffer_)) {
        LOG_DEBUG("%s", request_.path().c_str());
        response_.init(request_.path(), request_.isKeepAlive(), 200);
    } else {
        response_.init(request_.path(), false, 400);
    }

    response_.make(outbuffer_);

    iov_[0].iov_base = const_cast<char*>(outbuffer_.readStartPtr());
    iov_[0].iov_len = outbuffer_.read_cap();
    iovnum_ = 1;


    if(response_.fileSize() > 0  && response_.file()) {
        iov_[1].iov_base = response_.file();
        iov_[1].iov_len = response_.fileSize();
        iovnum_ = 2;
    }
    LOG_DEBUG("filesize:%d, %d  to %d", response_.fileSize() , iovnum_, iovRemain());
    return true;
}


bool HttpHandler::keepAlive() {
    return request_.isKeepAlive();
}