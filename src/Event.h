//
// Created by xwy on 2018/5/24.
//

#ifndef MEMDB_EVENT_H
#define MEMDB_EVENT_H

#include "Connection.h"
#include "utils/types.h"
#include <sys/epoll.h>
#include <vector>

class FDEvents {
    //connection pool
    Connection::connection_pool_t connections;
    //ready connections
    Connection::connection_pool_t ready_connections;
    //linsten socket
    Socket listen_sock;
    //max fds number
    static const int MAX_FDS = 8 * 1024;
    //epoll events
    epoll_event epoll_events[MAX_FDS];

public:
    bool isset(fd_t fd, int32_t flag);
    //set epoll event
    int set(fd_t fd, int32_t flag, int32_t data_num, void* data);
    //delete epoll event
    int del(fd_t fd);
    //
    int clr(fd_t fd, int32_t flag);
    //wait for events
    const std::vector<Connection*> &wait(int32_t timeout);
};


#endif //MEMDB_EVENT_H
