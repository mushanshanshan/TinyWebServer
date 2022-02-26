//
// Created by parallels on 2/26/22.
//

#include "httpresponse.h"

const std::unordered_map<int, std::string> Response::CODE_STATUS_ = {
        { 200, "OK" },
        { 400, "Bad Request" },
        { 403, "Forbidden" },
        { 404, "Not Found" },
};

const std::unordered_map<std::string , std::string> Response::FILE_TYPE_SUFFIX = {
        { ".html",  "text/html" },
        { ".xml",   "text/xml" },
        { ".xhtml", "application/xhtml+xml" },
        { ".txt",   "text/plain" },
        { ".rtf",   "application/rtf" },
        { ".pdf",   "application/pdf" },
        { ".word",  "application/nsword" },
        { ".png",   "image/png" },
        { ".gif",   "image/gif" },
        { ".jpg",   "image/jpeg" },
        { ".jpeg",  "image/jpeg" },
        { ".au",    "audio/basic" },
        { ".mpeg",  "video/mpeg" },
        { ".mpg",   "video/mpeg" },
        { ".avi",   "video/x-msvideo" },
        { ".gz",    "application/x-gzip" },
        { ".tar",   "application/x-tar" },
        { ".css",   "text/css "},
        { ".js",    "text/javascript "},
};


void Response::init(const std::string &path, bool isKeepAlive, int code) {
    if (!mmfile_) {} else { munmap(); }
    code_ = code;
    isKeepAlive_ = isKeepAlive;
    path_ = path;
    mmfile_ = nullptr;
    mmfilestat_ = { 0 };
}

void Response::redirect_error(int code) {
    code_ = code;
    std::string temppath("/");
    path_ = temppath + std::to_string(code_) + ".html";
    stat((srcdir_ + path_).data(), &mmfilestat_);
}


void Response::stateline_(Buffer &buffer) {
    buffer.put("HTTP/1.1 " + std::to_string(code_) + " " + CODE_STATUS_.find(code_)->second + "\r\n");
}


std::string Response::file_suffix() {
    std::string::size_type idx = path_.find_last_of('.');
    if(idx == std::string::npos) {
        return "text/plain";
    }
    std::string suffix = path_.substr(idx);
    if(FILE_TYPE_SUFFIX.count(suffix) == 1) {
        return FILE_TYPE_SUFFIX.find(suffix)->second;
    }
    return "text/plain";
}

void Response::header_(Buffer &buffer) {
    buffer.put("Connection: ");
    if(isKeepAlive_) {
        buffer.put("keep-alive\r\n");
        buffer.put("keep-alive: max=6, timeout=120\r\n");
    } else{
        buffer.put("close\r\n");
    }
    buffer.put("Content-type: " + file_suffix() + "\r\n");
}

void Response::content_(Buffer &buffer) {
    int srcFd = open((srcdir_ + path_).data(), O_RDONLY);

    int* mmRet = (int*)mmap(0, mmfilestat_.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);

    mmfile_ = (char*)mmRet;
    close(srcFd);
    buffer.put("Content-length: " + std::to_string(mmfilestat_.st_size) + "\r\n\r\n");
}



void Response::make(Buffer &buffer) {
    if(stat((srcdir_ + path_).data(), &mmfilestat_) < 0 || S_ISDIR(mmfilestat_.st_mode)) {
        redirect_error(404);
    }
    else if(!(mmfilestat_.st_mode & S_IROTH)) {
        redirect_error(403);
    }
    else if(code_ == -1) {
        code_ = 200;
    }

    stateline_(buffer);
    header_(buffer);
    content_(buffer);
}


void Response::munmap() {
    ::munmap(mmfile_, mmfilestat_.st_size);
    mmfile_ = nullptr;
}


size_t Response::fileSize() {
    return mmfilestat_.st_size;
}

char *Response::file() {
    return mmfile_;
}