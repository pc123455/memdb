//
// Created by xwy on 2018/5/24.
//

#include "Server.h"
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "Logger.h"
#include <algorithm>
#include "Connection.h"

const int Server::LISTENQ = 5;
const int Server::MAX_WAIT = 100;

Server::Server(): connection_pool(FDEvents::MAX_FDS), event(connection_pool) {
    
}

void Server::initialize(std::string ip, uint16_t port) {
    // initialize listen socket
    fd_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        Logger::error("socket error:");
        exit(1);
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);
    server_addr.sin_port = htons(port);

    if (bind(listen_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        Logger::error("bind error");
        exit(1);
    }
    listen(listen_fd, LISTENQ);

    event.initialize(listen_fd);
}

int Server::create_connection(fd_t fd, const sockaddr_in* client_addr) {
    if (fd > FDEvents::MAX_FDS) {
        Logger::error("too many connetions!");
        return -1;
    }

    if (connection_pool[fd] == nullptr) {
        connection_pool[fd] = new Connection();
    } else {
        //the connection is not closed
        if(!connection_pool[fd]->is_close()) {
            Logger::error("this connection is opened!");
            return -1;
        }
    }

    connection_pool[fd]->initialize(fd, client_addr);

    return 0;
}

int Server::destroy_connection(fd_t fd) {
    connection_pool[fd]->release();
    return -1;
}

void Server::serve() {
    for (;;) {
        auto expire_it = expire_time_map.begin();
        time_t wait_time = expire_it == expire_time_map.end() ? MAX_WAIT : expire_it->first - time(NULL);
        wait_time = std::max<time_t>(wait_time, 0);

        const Connection::connection_pool_t& reday_connections = event.wait(wait_time);

        //process timeouts connections
        time_t cur_time = time(NULL);
        for (expire_it = expire_time_map.begin(); expire_it != expire_time_map.end(); expire_it++) {
            if (expire_it->first > cur_time) {
                break;
            }
            //todo 处理超时连接
        }

        //process connections
        for (auto conn_it = reday_connections.begin(); conn_it != reday_connections.end(); conn_it++) {
            Connection* ready_conn = *conn_it;
            if (!ready_conn->is_close()) {
                if (ready_conn->flags & Connection::FLAG_READ) {
                    ready_conn->receive()
                }
            }
        }
    }

}