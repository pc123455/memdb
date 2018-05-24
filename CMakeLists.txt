cmake_minimum_required(VERSION 3.10)
project(memdb)

set(CMAKE_CXX_STANDARD 11)

add_executable(memdb src/main.cpp src/Logger.cpp src/Logger.h)

link_directories(/usr/local/lib)

target_link_libraries(memdb libglog.a pthread)
