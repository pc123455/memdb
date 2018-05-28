//
// Created by xwy on 2018/5/24.
//

#include "Server.h"
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Logger.h"

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