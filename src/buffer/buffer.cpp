//
// Created by mushan on 25/2/22.
//

#include "buffer.h"

Buffer::Buffer()
        :read_idx_(0),
         write_idx_(0),
         capacity_(BUFFER_SIZE),
         buffer_(BUFFER_SIZE){}


Buffer::~Buffer() = default;

const char *Buffer::readStartPtr() const {
    return bufferStartPtr() + read_idx_;
}

char *Buffer::readStartPtr()  {
    return bufferStartPtr() + read_idx_;
}


char *Buffer::writeStartPtr()  {
    return bufferStartPtr() + write_idx_;
}

const char *Buffer::bufferStartPtr() const{
    return &*buffer_.begin();
}

char *Buffer::bufferStartPtr() {
    return &*buffer_.begin();
}


size_t Buffer::read_cap() {
    return write_idx_ - read_idx_;
}

size_t Buffer::write_cap() {
    return capacity_ - write_idx_;
}


size_t Buffer::need_release_cap() {
    return read_idx_;
}

void Buffer::clean(size_t len) {
    read_idx_ += len;
}

void Buffer::clean(const char *end) {
    clean(end - readStartPtr());
}

void Buffer::clean() {
    read_idx_ = 0;
    write_idx_ = 0;
}

void Buffer::external_write(size_t len) {
    write_idx_ += len;
}

std::string Buffer::toStr() {
    std::string temp(readStartPtr(), read_cap());
    return temp;
}


std::string Buffer::toStrAndClean() {
    std::string temp(readStartPtr(), read_cap());
    clean();
    return temp;
}

void Buffer::extend_() {
    capacity_ *= BUFFER_EXTEND_RATIO;
    buffer_.resize(capacity_);

    arrange_();
}


void Buffer::arrange_() {
    size_t len = read_cap();
    std::copy(bufferStartPtr() + read_idx_, bufferStartPtr() + write_idx_, bufferStartPtr());
    read_idx_ = 0;
    write_idx_ = read_idx_ + len;
}




void Buffer::put(const std::string str) {
    put(str.data(), str.length());
}

void Buffer::put(const char *str, size_t len) {
    while (need_release_cap() + write_cap() < len) {
        extend_();
    }
    if (write_cap() < len) {
        arrange_();
    }
    std::copy(str, str+len, writeStartPtr());
    write_idx_ += len;
}

void Buffer::put(const void *str, size_t len) {
    put(static_cast<const char*>(str), len);
}

void Buffer::put(const Buffer &buffer) {
    put(buffer.readStartPtr(), read_cap());
}

ssize_t Buffer::read(int fd, int *error) {
    DEBUGCOUT("Buffer::read")
    char temp_buffer[BUFFER_FD_READER_BUFFER_SIZE];
    struct iovec iov[2];
    size_t writable = write_cap();

    iov[0].iov_base = &*buffer_.begin() + write_idx_;
    iov[0].iov_len = write_cap();
    iov[1].iov_base = temp_buffer;
    iov[1].iov_len = BUFFER_FD_READER_BUFFER_SIZE;

    ssize_t read_len = readv(fd, iov, 2);
    if (read_len < 0) {
        *error = errno;
    } else if (static_cast<size_t>(read_len) <= writable){
        write_idx_ += read_len;
    } else {
        write_idx_ = buffer_.size();
        put(temp_buffer, read_len - writable);
    }

    DEBUGCOUT("Buffer::read return")
    return read_len;
}


ssize_t Buffer::write(int fd, int *error) {
    size_t readable = read_cap();
    ssize_t write_len = ::write(fd, readStartPtr(), readable);
    if (write_len >= 0) {
        read_idx_ += write_len;
    } else {
        *error = errno;
    }
    return write_len;

}