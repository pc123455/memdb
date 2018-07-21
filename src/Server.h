//
// Created by xwy on 2018/5/24.
//

#ifndef MEMDB_SERVER_H
#define MEMDB_SERVER_H

#include <unordered_map>
#include <map>
#include <netinet/in.h>
#include "Connection.h"
#include "Proc.h"
#include "Event.h"
#include "db/DbEngine.h"
#include "config/Config.h"

class Server {
    //server address structure
    sockaddr_in server_addr;
    //conncetion pool
    std::vector<Connection*> connection_pool;
    //process function map
    std::unordered_map<std::string, Command> process_fun_map;
    //fd events
    FDEvents event;
    //timeouts set
    std::map<int64_t , Connection*> connection_expire_time_map;
    //db engine
    DbEngine* dbEngine;
    //process schedualer
    Proc proc;
    //configs
    Config* config;

    static const int LISTENQ;
    static const int64_t MAX_WAIT;
public:
    Server();

    ~Server();

    //initialize server
    void initialize(std::string ip, uint16_t port);
    //begin to serve
    void serve();
    //create a new connection
    int create_connection(fd_t fd, const sockaddr_in* client_addr);
    //initialize the process map
    int proc_initialize();
    //set a expire key into map
    int set_expire_map(int64_t time, const std::string& key);
    //destroy connection
    int destroy_connection(fd_t fd);
};


#endif //MEMDB_SERVER_H
