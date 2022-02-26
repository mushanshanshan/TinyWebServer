cmake_minimum_required(VERSION 3.14)
project(MyWebServer)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 14)


####################  G-TEST ####################
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
####################  G-TEST ####################

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


aux_source_directory(test TEST_SOURCES)
list(FILTER TEST_SOURCES INCLUDE REGEX "^.*_test.cpp$")

set(
        SRC_CODE
        src/buffer/buffer.cpp
        src/logger/logger.cpp
        src/timer/timer.cpp
        src/server/server.cpp
        src/utils/httprequest.cpp
        src/utils/httpresponse.cpp
        src/utils/httphandler.cpp
)


add_executable(
        ${PROJECT_NAME}_unit_test
        ${TEST_SOURCES}
        ./test/http_request_test.cpp
        ./test/http_response_test.cpp
        ${SRC_CODE}
        test/all.cpp
)

target_link_libraries(
        ${PROJECT_NAME}_unit_test
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_unit_test)


add_executable(
        ${PROJECT_NAME}
        ${SRC_CODE}
        src/server.cpp
)

target_link_libraries(
        ${PROJECT_NAME}
        Threads::Threads
)