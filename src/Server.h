//
// Created by xwy on 2018/5/24.
//

#ifndef MEMDB_SERVER_H
#define MEMDB_SERVER_H

#include "Connection.h"

class Server {
    //conncetion pool
    std::vector<Connection> connection_pool;
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
