cmake_minimum_required(VERSION 2.8)
project(ThreadPool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_subdirectory(test)
set(SOURCE_FILES thread_pool.cpp)
add_library(thread_pool ${SOURCE_FILES})