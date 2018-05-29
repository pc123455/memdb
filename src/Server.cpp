//
// Created by xwy on 2018/5/24.
//

#include "Server.h"
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "Logger.h"

Server::Server(): connection_pool(FDEvents::MAX_FDS) {
    
}

void Server::initialize(std::string ip, uint16_t port) {
    // initialize listen socket
    Socket listen_fd = socket(AF_INET, SOCK_STREAM, 0);
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