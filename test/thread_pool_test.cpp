//
// Created by mushan
//


#include <gtest/gtest.h>
#include <thread>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "../src/utils/threadpool.h"
#include "test_utils.h"
#include <sys/syscall.h>


TEST(ThreadTestTest,
new) {

ThreadPool *tp = new ThreadPool();
delete
tp;

}


TEST(ThreadTestTest, add_and_execute
) {

auto func = []() {
    std::cout << "test" << std::endl;
};

testing::internal::CaptureStdout();

ThreadPool *tp = new ThreadPool();
tp->
push(func);
std::this_thread::sleep_for(std::chrono::milliseconds(100)
);
std::string output = testing::internal::GetCapturedStdout();
delete
tp;

ASSERT_EQ(output,
"test\n");
}


TEST(ThreadTestTest, add_multiple_and_execute
) {

auto func = []() {
    std::cout << "T";
};

testing::internal::CaptureStdout();

ThreadPool *tp = new ThreadPool();
for (
int i = 0;
i<10000; i++) {
tp->
push(func);
}
std::this_thread::sleep_for(std::chrono::milliseconds(100)
);
std::cout.

flush();

std::string output = testing::internal::GetCapturedStdout();
delete
tp;

ASSERT_EQ(output, std::string(10000, 'T')
);
}