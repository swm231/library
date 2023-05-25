cmake_minimum_required(VERSION 3.14)

project(allocator)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)

# set(EXECUTABLE_OUTPUT_PATH ../bin)

# add_executable(main ${SRC_LIST})

set(LIBRARY_OUTPUT_PATH /home/swm_231/library/)

add_library(allo STATIC ${SRC_LIST})