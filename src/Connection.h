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
public:

    static const int FLAG_CLOSE     = 0;
    static const int FLAG_LISTEN    = 1;
    static const int FLAG_READ      = (1 << 1);
    static const int FLAG_WRITE     = (1 << 2);

    static const int STAGE_OK       = 0;
    static const int STAGE_AGAIN    = 1;
    static const int STAGE_ERROR    = -1;

    static const int INITIAL_BUFFER_SIZE = 10;
private:
    //socket handle
    fd_t fd;
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
    //read buffer
    std::vector<Byte> read_buff;
    //read buffer used
    size_t read_used;
    //read buffer current position
    std::vector<Byte>::iterator read_cur_pos;
    //write buffer
    std::vector<Byte> write_buff;
    //write buffer used
    size_t write_used;
    //write buffer current position
    std::vector<Byte>::iterator write_cur_pos;
    //temp buffer
    Byte* buffer;
    //temp buffer size
    size_t buffer_size;

public:
    //connection flags
    uint32_t flags;
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

    inline bool is_listen() { return (flags & FLAG_LISTEN) != 0; }

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
    int initialize(fd_t fd, const sockaddr_in* client_addr, uint32_t flags);

    /**
     * release a conntion
     * @return
     */
    int release();

    /**
     * receive data from net
     * @return
     */
    int receive();

    /**
     * send data from buffer
     * @return
     */
    int send();

    fd_t get_fd();
};


#endif //MEMDB_CONNECTION_H
