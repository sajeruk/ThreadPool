cmake_minimum_required(VERSION 2.8)
project(ThreadPoolTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(SOURCE_FILES test.cpp)
add_executable(thread_pool_test ${SOURCE_FILES})
target_link_libraries(thread_pool_test thread_pool)