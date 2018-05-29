//
// Created by xwy on 2018/5/24.
//

#include <unordered_map>
#include <arpa/inet.h>
#include "Event.h"
#include "Logger.h"

int FDEvents::EVENT_NONE = 0;
int FDEvents::EVENT_IN = 1 << 0;
int FDEvents::EVENT_PRI = 1 << 1;
int FDEvents::EVENT_OUT = 1 << 2;
int FDEvents::EVENT_HUP = 1 << 3;
int FDEvents::EVENT_ERR = 1 << 4;

void FDEvents::initialize(fd_t fd) {
    epollfd = epoll_create(MAX_FDS);
}

int FDEvents::set(fd_t fd, int32_t flag, int32_t data_num, void *data) {
    epoll_event ev;
    //if the connection is not exist or the connection is closed
    bool is_new_connection = (connections[fd] == nullptr) || (connections[fd]->is_close());
    int op_flag = is_new_connection ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
    if (flag & EVENT_IN) {
        ev.events |= EPOLLIN;
    }
    if (flag & EVENT_OUT) {
        ev.events |= EPOLLOUT;
    }

    //new connection pointer
    Connection* conn = connections[fd] == nullptr ? new Connection() : connections[fd];
    connections[fd] = conn;
    // todo connection对象初始化以及复用

    ev.data.ptr = conn;
    int ret = epoll_ctl(epollfd, op_flag, fd, &ev);
    if (ret == -1) {
        return -1;
    }

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

int FDEvents::accept(sockaddr_in& cliaddr, socklen_t& cliaddr_len) {
    int clifd = ::accept(listen_sock, (sockaddr*)&cliaddr, &cliaddr_len);
    if (clifd == -1) {
        Logger::error("accept error");
    } else {
        Logger::info("accept a new client") << inet_ntoa(cliaddr.sin_addr) << cliaddr.sin_port;
        return clifd;
    }
}