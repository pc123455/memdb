//
// Created by xwy on 2018/5/24.
//

#ifndef MEMDB_CONNECTION_H
#define MEMDB_CONNECTION_H

#include "Client.h"
#include "utils/types.h"
#include <functional>
#include <sys/socket.h>
#include <string>
#include <vector>
#include <cstring>
#include <netinet/in.h>


class Connection {
    //socket handle
    Socket fd;
    //send handle
    std::function<size_t (std::vector<Byte>&, size_t)> send_handle;
    //recieve handle
    std::function<size_t (std::vector<Byte>&, size_t)> receive_handle;
    //sent byte count
    size_t sent;
    //client sockaddr structure
    struct sockaddr sockaddr;
    //length of sockaddr
    socklen_t socklen;
    //client ip address
    std::string addr_text;
    //localhost sockaddr structure
    struct sockaddr local_sockaddr;
    //connection closed flag
    bool closed;

public:
    //connection pool type
    using connection_pool_t = std::vector<Connection*>;

    //client info
    Client* client;

    Connection();

    ~Connection();

    const std::function<size_t(std::vector<Byte>, size_t)> &get_send_handle() const;

    void set_send_handle(const std::function<size_t(std::vector<Byte>, size_t)> &send_handle);

    const std::function<size_t(std::vector<Byte>, size_t)> &get_recieve_handle() const;

    void set_recieve_handle(const std::function<size_t(std::vector<Byte>, size_t)> &recieve_handle);

    inline bool is_close() { return closed; }

    inline void set_close(bool close) {
        closed = close;
    }

    inline void set_clinet_addr(const struct sockaddr* s) {
        memcpy(&sockaddr, s, sizeof(sockaddr));
        socklen = sizeof(sockaddr);
    }

    /**
     * initialize a connection
     * @param fd
     * @param client_addr
     * @return
     */
    int initialize(fd_t fd, const sockaddr_in* client_addr);

    /**
     * release a conntion
     * @return
     */
    int release();
};


#endif //MEMDB_CONNECTION_H
