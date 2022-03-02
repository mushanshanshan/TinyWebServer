//
// Created by mushan
//


#ifndef MY_WEB_SERVER_TEST_UTILS_H
#define MY_WEB_SERVER_TEST_UTILS_H

struct Num_ {
    int i;
    int ref;

    bool operator<(const Num_ &n) {
        return i < n.i;
    }

    int get_ref() { return ref; }
};

#endif //MY_WEB_SERVER_TEST_UTILS_H
