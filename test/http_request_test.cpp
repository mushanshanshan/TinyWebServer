//
// Created by mushan
//


#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <stdlib.h>
#include "../src/utils/httprequest.h"
#include "test_utils.h"


TEST(HttpRequestTest,
new) {

Request re;

ASSERT_EQ(1,1);

}

TEST(HttpRequestTest, parse_and_check_path
) {

Request re;
Buffer buffer;
std::string request = "GET /simple.htm HTTP/1.1\r\n"
                      "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\n"
                      "Accept-Language: zh-cn\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n"
                      "Host: localhost:8080\r\n"
                      "Connection: Keep-Alive\r\n"
                      "";
buffer.
put(request);
re.
parse(buffer);

ASSERT_EQ(re
.

path(),

"/simple.htm");
}


TEST(HttpRequestTest, parse_and_check_method
) {

Request re;
Buffer buffer;
std::string request = "GET /simple.htm HTTP/1.1\r\n"
                      "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\n"
                      "Accept-Language: zh-cn\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n"
                      "Host: localhost:8080\r\n"
                      "Connection: Keep-Alive\r\n"
                      "";
buffer.
put(request);
re.
parse(buffer);

ASSERT_EQ(re
.

method(),

"GET");
}

TEST(HttpRequestTest, parse_and_check_version
) {

Request re;
Buffer buffer;
std::string request = "GET /simple.htm HTTP/1.1\r\n"
                      "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\n"
                      "Accept-Language: zh-cn\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n"
                      "Host: localhost:8080\r\n"
                      "Connection: Keep-Alive\r\n"
                      "";
buffer.
put(request);
re.
parse(buffer);

ASSERT_EQ(re
.

version(),

"1.1");
}

TEST(HttpRequestTest, parse_and_check_head
) {

Request re;
Buffer buffer;
std::string request = "GET /simple.htm HTTP/1.1\r\n"
                      "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\n"
                      "Accept-Language: zh-cn\r\n"
                      "Accept-Encoding: gzip, deflate\r\n"
                      "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n"
                      "Host: localhost:8080\r\n"
                      "Connection: Keep-Alive\r\n"
                      "";
buffer.
put(request);
re.
parse(buffer);

ASSERT_EQ(re
.

head()

.find("Accept-Language")->second, "zh-cn");
}