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
#include <sys/fcntl.h>
#include "Connection.h"
#include "db/LevelDbEngine.h"

const int Server::LISTENQ = 5;
const int Server::MAX_WAIT = 100;

Server::Server(): connection_pool(FDEvents::MAX_FDS), event(connection_pool), dbEngine(nullptr) {
    
}

Server::~Server() {}

void Server::initialize(std::string ip, uint16_t port) {
    //db initialize
    dbEngine = new LevelDbEngine();
    int res = dbEngine->initialize();
    if (res != DbEngine::DB_OK) {
        Logger::error("db initialize error!");
        exit(1);
    }
    // initialize listen socket
    fd_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        Logger::error("socket error:");
        exit(1);
    }
    //set socket as non-blocking
    int flags = fcntl(listen_fd, F_GETFL, 0);
    fcntl(listen_fd, F_SETFL, flags | O_NONBLOCK);

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
    //
    connection_pool[listen_fd] = new Connection();
    connection_pool[listen_fd]->initialize(listen_fd, nullptr, Connection::FLAG_LISTEN);
    //add listen fd to epoll
    event.set(listen_fd, FDEvents::EVENT_IN, 0, nullptr);
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

    connection_pool[fd]->initialize(fd, client_addr, Connection::FLAG_READ | Connection::FLAG_WRITE);

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

        const Connection::connection_pool_t& ready_connections = event.wait(wait_time);

        //process timeouts connections
        time_t cur_time = time(NULL);
        for (expire_it = expire_time_map.begin(); expire_it != expire_time_map.end(); expire_it++) {
            if (expire_it->first > cur_time) {
                break;
            }
            //todo 处理超时连接
        }

        //process connections
        for (auto conn_it = ready_connections.begin(); conn_it != ready_connections.end(); conn_it++) {
            Connection* ready_conn = *conn_it;
            //accept the new connection
            if (ready_conn->is_listen()) {
                sockaddr_in client_addr;
                socklen_t addr_len;
                while(true) {
                    fd_t client_fd = event.accept(client_addr, addr_len);
                    if (client_fd != -1) {
                        create_connection(client_fd, &client_addr);
                        event.set(client_fd, FDEvents::EVENT_IN, 0, nullptr);
                    } else {
                        break;
                    }
                }
                continue;
            }
            if (!ready_conn->is_close()) {
                if (ready_conn->flags & Connection::FLAG_READ) {
                    int res = ready_conn->receive();
                    switch (res) {
                        case Connection::STAGE_OK:
                            //data read complete, begin the next stage process
                            //todo 网络数据读取完毕，开始进行下一阶段的处理
                            int res = proc.process(ready_conn, dbEngine);
                            if (res == Proc::PROCESS_OK) {

                            }
//                            ready_conn->send();
                            break;
                        case Connection::STAGE_AGAIN:
                            //todo 网络数据尚未全部读取，等待新的数据
                            //data read not complete, wait new data
                            event.set(ready_conn->get_fd(), FDEvents::EVENT_IN, 0, nullptr);
                            break;
                        default:
                            break;
                    }
                }
                if (ready_conn->flags & Connection::FLAG_WRITE) {
                    int res = ready_conn->send();
                    switch (res) {
                        case Connection::STAGE_OK:
                            event.set(ready_conn->get_fd(), FDEvents::EVENT_IN, 0, nullptr);
                            //if all data is sent, the write buffer will be cleared.
                            ready_conn->clear_write_buffer();
                            ready_conn->flags &= ~Connection::FLAG_WRITE;
                            break;
                        case Connection::STAGE_AGAIN:
                            event.set(ready_conn->get_fd(), FDEvents::EVENT_IN | FDEvents::EVENT_OUT, 0, nullptr);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        event.reset_ready_conncetions();
    }

}