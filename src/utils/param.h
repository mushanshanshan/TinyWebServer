//
// Created by mushan
//

#ifndef MY_WEB_SERVER_CONFIG_H
#define MY_WEB_SERVER_CONFIG_H



/*!
 ****** Global Server Parameters ******
 */
#define SERVER_PORT 80
#define SERVER_TIMEOUT 60000
#define SERVER_LOG_LEVEL -1
#define SERVER_REACTOR_BACKLOG 128
#define SERVER_USE_LOGGER true
#define SERVER_USE_ELEG_DISCONNECT true
#define SERVER_RESOURCE_DIR  "./data/"
#define THREAD_POOL_THREAD_NUM  2
#define EPOLL_LENGTH 100
#define EPOLL_MAX_EVENTS_LENGTH 10
#define EPOLL_TRIGGER_METHOD "ET"
#define EPOLL_TIME_OUT -1


/*!
 ****** Global Buffer Parameters ******
 */
#define BUFFER_SIZE 1000
#define BUFFER_EXTEND_RATIO 1.5
#define BUFFER_FD_READER_BUFFER_SIZE 5000



/*!
 ****** Global Logger Parameters ******
 */
#define BLOCKING_QUEUE_LENGTH 200
#define LOGGER_PATH "./log"
#define LOGGER_LEVEL 0;
#define LOGGER_QUEUE_CAPACITY 200
#define LOGGER_FILE_NAME_MAX_LEN 30
#define LOGGER_FILE_SUFFIX ".log"



/*!
 ****** Global Timer Parameters ******
 */
#define TIMER_HEAP_SIZE  100


#endif //MY_WEB_SERVER_CONFIG_H
