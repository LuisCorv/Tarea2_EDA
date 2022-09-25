
cmake_minimum_required(VERSION 3.24.0)

project(sort
VERSION 1.0
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

#add_subdirectory(src)
#add_subdirectory(tests)

add_executable(test test/test_parenthesis.cpp src/node.cpp src/stack.cpp)
target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/include)


# add the executable