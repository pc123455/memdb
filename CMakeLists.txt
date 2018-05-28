cmake_minimum_required(VERSION 3.10)
project(memdb)

set(CMAKE_CXX_STANDARD 11)

add_executable(memdb src/main.cpp src/Logger.cpp src/Logger.h src/Event.cpp src/Event.h src/Event.cpp src/Event.h src/Connection.cpp src/Connection.h src/Server.cpp src/Server.h src/Application.cpp src/Application.h src/Client.cpp src/Client.h src/utils/types.h src/Proc.cpp src/Proc.h)

link_directories(/usr/local/lib)

target_link_libraries(memdb libglog.a pthread)
