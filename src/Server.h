//
// Created by xwy on 2018/5/24.
//

#ifndef MEMDB_SERVER_H
#define MEMDB_SERVER_H

#include <unordered_map>
#include <netinet/in.h>
#include "Connection.h"
#include "Proc.h"
#include "Event.h"

class Server {
    //server address structure
    sockaddr_in server_addr;
    //conncetion pool
    std::vector<Connection*> connection_pool;
    //process function map
    std::unordered_map<std::string, Command> process_fun_map;
    //fd events
    FDEvents event;

public:
    Server();

    ~Server();

    //initialize server
    void initialize(std::string ip, uint16_t port);
    //begin to serve
    void serve();
    //create a new connection
    int create_connection(fd_t fd, const sockaddr_in* client_addr);
    //destroy connection
    int destroy_connection(fd_t fd);
};


#endif //MEMDB_SERVER_H
