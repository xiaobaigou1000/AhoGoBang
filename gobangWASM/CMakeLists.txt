cmake_minimum_required(VERSION 3.12)
project(GoBangWASM)

set(MyBuildFlag "${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sMODULARIZE=1 -sEXPORT_ES6=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind")
set(CMAKE_CXX_STANDARD 17)

add_executable(gobang gobang.cpp)
target_include_directories(gobang PUBLIC include)