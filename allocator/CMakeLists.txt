cmake_minimum_required(VERSION 3.14)

project(allocator)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(LIBRARY_OUTPUT_PATH /home/swm_231/library/lib)

add_library(allo STATIC ${SRC_LIST})


# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
# add_executable(test ${SRC_LIST})