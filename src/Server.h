//
// Created by xwy on 2018/5/24.
//

#ifndef MEMDB_SERVER_H
#define MEMDB_SERVER_H

#include <bits/unordered_map.h>
#include <unordered_map>
#include "Connection.h"

class Server {
    //conncetion pool
    std::vector<Connection*> connection_pool;
    //process function map
    std::unordered_map<std::string,
    //
public:
    Server();

    ~Server();

    //initialize server
    void initialize();
    //begin to serve
    void serve();
};


#endif //MEMDB_SERVER_H
