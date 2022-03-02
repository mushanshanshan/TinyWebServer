//
// Created by mushan
//

#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <stdlib.h>
#include "../src/utils/blockingqueue.h"
#include "test_utils.h"


void test_push_back(BlockingQueue<int> *b) {
    for (int i=0; i<50; i++) {
        b->push_back(rand());
    }
}

void test_push_front(BlockingQueue<int> *b) {
    for (int i=0; i<50; i++) {
        b->push_front(rand());
    }
}


TEST(BlockingQueueTest, new_and_empty) {

    BlockingQueue<int>* bq = new BlockingQueue<int>();

    ASSERT_EQ(bq->is_empty(), true);

    delete(bq);
}


TEST(BlockingQueueTest, push) {

    BlockingQueue<int> bq{};

    for (int i=0; i<100; i++) {
        bq.push_back(i);
    }

    ASSERT_EQ(bq.size(), 100);

}


TEST(BlockingQueueTest, push_back_and_pop) {

    BlockingQueue<int> bq{};

    for (int i=0; i<100; i++) {
        bq.push_back(i);
    }

    for (int i=0; i<50; i++) {
        bq.pop();
    }

    ASSERT_EQ(bq.size(), 50);

}


TEST(BlockingQueueTest, push_front_and_pop) {

    BlockingQueue<int> bq{};

    for (int i=0; i<60; i++) {
        bq.push_front(i);
    }

    for (int i=0; i<50; i++) {
        bq.pop();
    }

    ASSERT_EQ(bq.front(), 9);

}


TEST(BlockingQueueTest, single_thread_push) {

    BlockingQueue<int>* bq = new BlockingQueue<int>();

    auto t = std::thread(test_push_front, bq);

    t.join();

    ASSERT_EQ(bq->size(), 50);

}


TEST(BlockingQueueTest, multi_thread_push) {

    BlockingQueue<int>* bq = new BlockingQueue<int>();

    std::vector<std::thread> workers;

    auto func = [](BlockingQueue<int>* b) {
        for (int i=0; i<50; i++) {
            b->push_back(i);
        }
    };

    for (int i=0; i<16; i++) {
        workers.push_back(std::thread (func, bq));
    }

    for (int i=0; i<16; i++) {
        workers[i].join();
    }

    ASSERT_EQ(bq->size(), 800);

}


TEST(BlockingQueueTest, multi_thread_push_to_full) {

    std::vector<std::thread> workers;
    BlockingQueue<int>* bq = new BlockingQueue<int>();

    auto func = [](BlockingQueue<int>* b) {
        for (int i=0; i<50; i++) {
            b->push_back(i);
        }
    };

    for (int i=0; i<20; i++) {
        workers.push_back(std::thread (func, bq));
    }

    for (int i=0; i<20; i++) {
        workers[i].join();
    }

    ASSERT_EQ(bq->is_full(), true);

}

TEST(BlockingQueueTest, multi_thread_push_and_pop_to_empty) {

    std::vector<std::thread> workers;
    BlockingQueue<int>* bq = new BlockingQueue<int>();

    auto func = [](BlockingQueue<int>* b) {
        for (int i=0; i<50; i++) {
            b->push_back(i);
        }
        for (int i=0; i<50; i++) {
            b->pop();
        }
    };

    for (int i=0; i<20; i++) {
        workers.push_back(std::thread (func, bq));
    }

    for (int i=0; i<20; i++) {
        workers[i].join();
    }

    ASSERT_EQ(bq->is_empty(), true);

}