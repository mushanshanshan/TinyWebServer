//
// Created by mushan on 25/2/22.
//

#ifndef MY_PROJECT_BUFFER_H
#define MY_PROJECT_BUFFER_H




#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/uio.h>
#include <vector>
#include <atomic>
#include <assert.h>
#include "../utils/param.h"

class Buffer {
private:
    std::vector<char> buffer_;
    std::atomic<size_t> read_idx_;
    std::atomic<size_t> write_idx_;
    size_t capacity_;

public:
    Buffer();
    ~Buffer();

    const char* bufferStartPtr() const;
    char* bufferStartPtr() ;
    const char* readStartPtr() const;
    char* readStartPtr() ;
    const char* writeStartPtr() const;
    char* writeStartPtr() ;



    size_t read_cap();
    size_t write_cap();
    size_t need_release_cap();


    void external_write(size_t len);
    void clean(size_t len);
    void clean(const char* end);
    void clean();


    std::string toStr();
    std::string toStrAndClean();

    void put(const std::string str);
    void put(const char* str, size_t len);
    void put(const void* str, size_t len);
    void put(const Buffer& buffer);


    ssize_t read(int fd, int* error);
    ssize_t write(int fd, int *error);


private:
    void arrange_();
    void extend_();

};


#endif //MY_PROJECT_BUFFER_H
