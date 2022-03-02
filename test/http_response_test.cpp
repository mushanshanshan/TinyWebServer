//
// Created by mushan
//


#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include "../src/utils/httpresponse.h"
#include "test_utils.h"
#include <stdio.h>

using namespace std;

vector <string> split(const string &str, const string &delim) {
    vector <string> res;
    if ("" == str) return res;

    string strs = str + delim;
    size_t pos;
    size_t size = strs.size();

    for (int i = 0; i < size; ++i) {
        pos = strs.find(delim, i);
        if (pos < size) {
            string s = strs.substr(i, pos - i);
            res.push_back(s);
            i = pos + delim.size() - 1;
        }

    }
    return res;
}


TEST(ResponseTest, compile
) {

}


TEST(ResponseTest, make_response_404_and_keep_alive
) {

Response response;
Buffer buff;
std::string path = "NONE";
response.
init(path,
true, 200);
response.
make(buff);
std::string str = buff.toStrAndClean();
std::vector <std::string> vec = split(str, "\r\n");


ASSERT_EQ(vec[0],
"HTTP/1.1 404 Not Found");
ASSERT_EQ(vec[1],
"Connection: keep-alive");
ASSERT_EQ(vec[3],
"Content-type: text/html");
ASSERT_EQ(vec[4],
"Content-length: 0");
}


TEST(ResponseTest, make_response_404_and_not_keep_alive
) {

Response response;
Buffer buff;
std::string path = "NONE";
response.
init(path,
false, 200);
response.
make(buff);
std::string str = buff.toStrAndClean();
std::vector <std::string> vec = split(str, "\r\n");

ASSERT_EQ(vec[1],
"Connection: close");
}


TEST(ResponseTest, make_response_200
) {

FILE *fp = fopen("./data/test", "w");
if (fp == nullptr) {
system("mkdir data");
FILE *fp = fopen("./data/test", "w");
}

string temp = "testtest";
fputs(temp
.

c_str(), fp

);
fclose(fp);

Response response;
Buffer buff;
std::string path = "test";
response.
init(path,
false, 200);
response.
make(buff);
std::string str = buff.toStrAndClean();
std::vector <std::string> vec = split(str, "\r\n");

ASSERT_EQ(vec[0],
"HTTP/1.1 200 OK");
ASSERT_EQ(vec[2],
"Content-type: text/plain");
ASSERT_EQ(vec[3],
"Content-length: 8");
}