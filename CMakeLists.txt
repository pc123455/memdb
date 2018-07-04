cmake_minimum_required(VERSION 3.10)
project(memdb)

set(CMAKE_CXX_STANDARD 11)

add_executable(memdb src/main.cpp src/Logger.cpp src/Logger.h src/Event.cpp src/Event.h src/Event.cpp src/Event.h src/Connection.cpp src/Connection.h src/Server.cpp src/Server.h src/Application.cpp src/Application.h src/Client.cpp src/Client.h src/utils/types.h src/Proc.cpp src/Proc.h src/utils/Buffer.cpp src/utils/Buffer.h src/ProtocolParser.cpp src/ProtocolParser.h src/db/DbEngine.cpp src/db/DbEngine.h src/db/LevelDbEngine.cpp src/db/LevelDbEngine.h src/utils/Error.cpp src/utils/Error.h src/proc/proc_string.cpp src/db/constant.h src/db/t_string.cpp src/proc/proc_connection.cpp src/utils/response.h src/config/Config.cpp src/config/Config.h src/utils/Common.cpp src/utils/Common.h)

link_directories(/usr/local/lib)
#link_directories(/home/python/CLionProjects/leveldb/build)
include_directories(/Users/xueweiyao/Documents/Clionprojects/leveldb/include)
include_directories(/home/python/CLionProjects/leveldb/include)

target_link_libraries(memdb libglog.a pthread libleveldb.a)