//
// Created by mushan on 25/2/22.
//

#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <stdlib.h>
#include "../src/buffer/buffer.h"
#include "test_utils.h"



TEST(BufferTest, new) {

    Buffer* b = new Buffer();

    ASSERT_EQ(b->read_cap(), 0);
    ASSERT_EQ(b->write_cap(), BUFFER_SIZE);
}


TEST(BufferTest, put_1) {

    Buffer* b = new Buffer();
    b->put("abcdefg");
    std::string str = b->toStr();
    ASSERT_EQ(str, "abcdefg");
}

TEST(BufferTest, multiple_put) {

    Buffer* b = new Buffer();
    b->put("abcdefg");
    b->put("h");
    std::string str = b->toStr();
    ASSERT_EQ(str, "abcdefgh");
}

TEST(BufferTest, write_and_read_by_fd) {
    int* err;
    FILE* fp_ = fopen("./test", "w+");
    int fdw = fileno(fp_);
    Buffer* b = new Buffer();
    b->put("abcdefg");
    b->put("\n\0");
    b->write(fdw, err);
    b->clean();
    fclose(fp_);

    FILE* fpr_ = fopen("./test", "r+");
    int fdr = fileno(fpr_);
    b->read(fdr, err);
    std::string str = b->toStr();
    fclose(fpr_);

    ASSERT_EQ(str, "abcdefg\n\0");
}


TEST(BufferTest, write_and_extend_arrange) {

    Buffer* b = new Buffer();

    for (int i=0; i<10000; i++) {
        b->put("abcdefghij");
    }

    ASSERT_EQ(b->read_cap(), 10000 * 10);
}