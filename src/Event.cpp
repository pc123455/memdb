//
// Created by xwy on 2018/5/24.
//

#include <unordered_map>
#include <arpa/inet.h>
#include <exception>
#include "Event.h"
#include "Logger.h"

int FDEvents::EVENT_NONE = 0;
int FDEvents::EVENT_IN = 1 << 0;
int FDEvents::EVENT_PRI = 1 << 1;
int FDEvents::EVENT_OUT = 1 << 2;
int FDEvents::EVENT_HUP = 1 << 3;
int FDEvents::EVENT_ERR = 1 << 4;

FDEvents::FDEvents(Connection::connection_pool_t &connection_pool): connections(connection_pool) {

}

void FDEvents::initialize(fd_t fd) {
    epollfd = epoll_create(MAX_FDS);
}

int FDEvents::set(fd_t fd, int32_t flag, int32_t data_num, void *data) {
    epoll_event ev;
    Connection* conn = connections[fd];
    //if the connection is not exist or the connection is closed
    bool is_new_connection = fd_connection_map.find(fd) == fd_connection_map.end();
    int op_flag = is_new_connection ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;
    if (flag & EVENT_IN) {
        ev.events |= EPOLLIN;
    }
    if (flag & EVENT_OUT) {
        ev.events |= EPOLLOUT;
    }

    ev.data.fd = fd;
    int ret = epoll_ctl(epollfd, op_flag, fd, &ev);
    if (ret == -1) {
        return -1;
    }

    //insert fd to the set
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

const Connection::connection_pool_t& FDEvents::wait(time_t timeout) {
    ready_connections.clear();
    int ret = epoll_wait(epollfd, epoll_events, EPOLL_EVENTS, timeout);
    if (ret > 0) {
        for(int i = 0; i < ret; i++) {
            epoll_event& ev = epoll_events[i];
            fd_t fd = ev.data.fd;
            //add connection to ready array
            ready_connections.push_back(fd_connection_map[fd]);
        }
    } else if (ret == 0) {
        //return is equal to 0 means connection timeouts
        return ready_connections;
    } else if (ret < 0) {
        //return is small than 0 mean errors happened
        Logger::error("epoll error");
        //todo epoll error
        throw new std::runtime_error("epoll error");
    }
    return ready_connections;
}