//
// Created by mushan
//


#include <gtest/gtest.h>
#include <thread>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "../src/timer/timer.h"
#include <ctime>
#include "test_utils.h"
#include <sys/syscall.h>


TEST(TimerTest, push
) {

auto func = []() {
    std::cout << "test" << std::endl;
};

Timer t = Timer();

t.push(10, 10, func);

testing::internal::CaptureStdout();

t.execute(10);
std::string output = testing::internal::GetCapturedStdout();

ASSERT_EQ("test\n", output);
}


TEST(TimerTest, push_and_tick
) {

auto func = []() {
    std::cout << "test" << std::endl;
};

Timer t = Timer();

t.push(10, 0, func);

testing::internal::CaptureStdout();

std::this_thread::sleep_for(std::chrono::milliseconds(100)
);
t.

tick();

std::string output = testing::internal::GetCapturedStdout();

ASSERT_EQ("test\n", output);
}

TEST(TimerTest, multi_push_and_tick_with_same_fd
) {

auto func = []() {
    std::cout << "test" << std::endl;
};

Timer t = Timer();

t.push(10, 0, func);
t.push(10, 0, func);
t.push(10, 0, func);
t.push(10, 0, func);

testing::internal::CaptureStdout();

std::this_thread::sleep_for(std::chrono::milliseconds(100)
);
t.

tick();

std::string output = testing::internal::GetCapturedStdout();

ASSERT_EQ("test\n", output);
}


TEST(TimerTest, multi_push_and_tick_with_diff_fd
) {

auto func = []() {
    std::cout << "test" << std::endl;
};

Timer t = Timer();

t.push(10, 0, func);
t.push(11, 0, func);
t.push(12, 0, func);
t.push(13, 0, func);

testing::internal::CaptureStdout();

std::this_thread::sleep_for(std::chrono::milliseconds(100)
);
t.

tick();

std::string output = testing::internal::GetCapturedStdout();

ASSERT_EQ("test\ntest\ntest\ntest\n", output);
}


TEST(TimerTest, modify
) {

auto func = []() {
    std::cout << "test" << std::endl;
};

auto func2 = []() {
    std::cout << "test" << std::endl;
};

Timer t = Timer();

t.push(10, 0, func);
t.push(10, 0, func2);

testing::internal::CaptureStdout();

std::this_thread::sleep_for(std::chrono::milliseconds(100)
);
t.

tick();

std::string output = testing::internal::GetCapturedStdout();

ASSERT_EQ("test\n", output);
}


TEST(TimerTest, wait_for_expries
) {

auto func = []() {
    std::cout << "test" << std::endl;
};


Timer t = Timer();

t.push(10, 90, func);


testing::internal::CaptureStdout();

std::this_thread::sleep_for(std::chrono::milliseconds(100)
);
t.

tick();

std::string output = testing::internal::GetCapturedStdout();

ASSERT_EQ("test\n", output);
}