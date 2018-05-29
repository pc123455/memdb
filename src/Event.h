//
// Created by xwy on 2018/5/24.
//

#ifndef MEMDB_EVENT_H
#define MEMDB_EVENT_H

#include "Connection.h"
#include "utils/types.h"
#include <sys/epoll.h>
#include <vector>
#include <unordered_map>

class FDEvents {
    //max fds number
public:
    static const int MAX_FDS = 8 * 1024;

private:
    //connection pool
    Connection::connection_pool_t connections;
    //ready connections
    Connection::connection_pool_t ready_connections;
    //linsten socket
    fd_t listen_sock;
    //epoll events
    epoll_event epoll_events[MAX_FDS];
    //
    int epollfd;
    //fd connection map
    std::unordered_map<fd_t, Connection*> fd_connection_map;

public:

    static int EVENT_NONE;
    static int EVENT_IN;
    static int EVENT_PRI;
    static int EVENT_OUT;
    static int EVENT_HUP;
    static int EVENT_ERR;

public:
    void initialize(Socket fd);

    bool isset(fd_t fd, int32_t flag);
    //set epoll event
    int set(fd_t fd, int32_t flag, int32_t data_num, void* data);
    //delete epoll event
    int del(fd_t fd);
    //
    int clr(fd_t fd, int32_t flag);
    //wait for events
    const std::vector<Connection*> &wait(int32_t timeout);
    //accept a new connection
    int accept(sockaddr_in& cliaddr, socklen_t& cliaddr_len);
};


#endif //MEMDB_EVENT_H
