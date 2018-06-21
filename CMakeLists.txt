cmake_minimum_required(VERSION 3.10)
project(memdb)

set(CMAKE_CXX_STANDARD 11)

add_executable(memdb src/main.cpp src/Logger.cpp src/Logger.h src/Event.cpp src/Event.h src/Event.cpp src/Event.h src/Connection.cpp src/Connection.h src/Server.cpp src/Server.h src/Application.cpp src/Application.h src/Client.cpp src/Client.h src/utils/types.h src/Proc.cpp src/Proc.h src/utils/Buffer.cpp src/utils/Buffer.h src/ProtocolParser.cpp src/ProtocolParser.h src/db/DbEngine.cpp src/db/DbEngine.h src/db/LevelDbEngine.cpp src/db/LevelDbEngine.h src/utils/Error.cpp src/utils/Error.h src/proc/proc_kv.cpp src/db/constant.h src/db/t_kv.cpp)

link_directories(/usr/local/lib)
include_directories(/Users/xueweiyao/Documents/Clionprojects/leveldb/include)
include_directories(/home/python/CLionProjects/leveldb/include)
target_link_libraries(memdb libglog.a pthread)
