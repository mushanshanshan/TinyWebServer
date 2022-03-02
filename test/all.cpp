//
// Created by mushan
//



#include <gtest/gtest.h>
#include "../src/logger/logger.h"

int main(int argc, char **argv) {
    Logger::instancePtr()->init();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}