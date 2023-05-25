cmake_minimum_required(VERSION 3.14)

project(thread)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(LIBRARY_OUTPUT_PATH /home/swm_231/library/)

add_library(threadpool STATIC ${SRC_LIST})