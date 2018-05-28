//
// Created by xwy on 2018/5/24.
//

#include <unordered_map>
#include "Event.h"
#include "Logger.h"

void FDEvents::initialize(fd_t fd) {
    epollfd = epoll_create(MAX_FDS);
}

int FDEvents::set(fd_t fd, int32_t flag, int32_t data_num, void *data) {
    epollfd_event ev;
    auto it = fd_connection_map.find(fd);
    //if the connection is not exist or the connection is closed
    bool is_new_connection = it == fd_connection_map.end();
    int op_flag = is_new_connection ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
    if (flag & EVENT_IN) {
        ev |= EPOLLIN;
    }
    if (flag & EVENT_OUT) {
        ev |= EPOLLOUT;
    }

    ev.data.fd = fd;
    int ret = epoll_ctl(epollfd, op_flag, fd, &ev);
    if (ret == -1) {
        return -1;
    }


    //new connection pointer
    Connection* conn = it == fd_connection_map.end() ? new Connection() : it->second;
    // todo connection对象初始化以及复用

    conn->set_close(false);
    if(is_new_connection) {
        fd_connection_map[fd] = conn;
    }

    return 0;
}

int FDEvents::del(fd_t fd) {
    epoll_event ev;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);

    if (ret == -1) {
        return -1;
    }

    auto it = fd_connection_map.find(fd);
    if (it != fd_connection_map.end()) {
        it->second->set_close(true);
    }
    return 0;
}