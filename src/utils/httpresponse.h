//
// Created by mushan on 26/2/22.
//

#ifndef MY_PROJECT_HTTPRESPONSE_H
#define MY_PROJECT_HTTPRESPONSE_H


#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../buffer/buffer.h"
#include "../logger/logger.h"
#include "../utils/param.h"

class Response {
private:
    std::string path_;
    std::string srcdir_ = SERVER_RESOURCE_DIR;

    bool isKeepAlive_;
    int code_;
    char* mmfile_;
    struct stat mmfilestat_;

    const static std::unordered_map<int, std::string> CODE_STATUS_;
    const static std::unordered_map<std::string , std::string> FILE_TYPE_SUFFIX;



public:
    Response() = default;

    ~Response() {this->munmap();}

    void init(const std::string& path, bool isKeepAlive, int code);
    void make(Buffer &buffer);
    void munmap();

    char* file();
    size_t fileSize();


private:


    void stateline_(Buffer &buffer);
    void header_(Buffer &buffer);
    void content_(Buffer &buffer);

    void redirect_error(int code);
    std::string file_suffix();


};


#endif //MY_PROJECT_HTTPRESPONSE_H
