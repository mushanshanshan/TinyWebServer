//
// Created by mushan on 25/2/22.
//

#include <gtest/gtest.h>
#include <thread>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "../src/logger/logger.h"
#include "test_utils.h"
#include <sys/syscall.h>



TEST(LoggerTest, do_log_debug) {

    LOG_DEBUG("test");


}


TEST(LoggerTest, do_log_info) {

    LOG_INFO("test");


}

TEST(LoggerTest, do_log_warn) {

    LOG_WARN("test");


}

TEST(LoggerTest, do_log_error) {

    LOG_ERROR("test");

}


TEST(LoggerTest, format_log_test) {

    for (int i=0; i<10; i++) {
        LOG_INFO("%04d", i);
    }

}


TEST(LoggerTest, bulk_format_log_test) {

    for (int i=0; i<1000; i++) {
        LOG_INFO("####################%04d####################", i);
    }

}


TEST(LoggerTest, multi_thread_bulk_format_log_test) {

    std::vector<std::thread> workers;

    auto func = []() {
        for (int i=0; i<400; i++) {
            LOG_INFO("####################%04d--%04d####################", std::this_thread::get_id(), i);
        }

    };

    for (int i=0; i<20; i++) {
        workers.push_back(std::thread (func));
    }

    for (int i=0; i<20; i++) {
        workers[i].join();
    }

}