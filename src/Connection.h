//
// Created by xwy on 2018/5/24.
//

#ifndef MEMDB_CONNECTION_H
#define MEMDB_CONNECTION_H

#include "Event.h"
#include <functional>
#include <sys/socket.h>
#include <string>
#include <vector>

using Socket = int;
using Byte = u_char;

class Connection {
    //read event
    Event read;
    //write event
    Event write;
    //socket handle
    Socket fd;
    //send handle
    std::function<size_t (std::vector<Byte>, size_t)> send_handle;
    //recieve handle
    std::function<size_t (std::vector<Byte>, size_t)> recieve_handle;
    //sent byte count
    size_t sent;
    //client sockaddr structure
    sockaddr sockaddr;
    //length of sockaddr
    socklen_t socklen;
    //client ip address
    std::string addr_text;
    //localhost sockaddr structure
    sockaddr local_sockaddr;
    //buffer
    std::vector<Byte> buffer;
    //connection closed flag
    unsigned close;

public:
    ~Connection();

    const std::function<size_t(std::vector<Byte>, size_t)> &get_send_handle() const;

    void set_send_handle(const std::function<size_t(std::vector<Byte>, size_t)> &send_handle);

    const std::function<size_t(std::vector<Byte>, size_t)> &get_recieve_handle() const;

    void set_recieve_handle(const std::function<size_t(std::vector<Byte>, size_t)> &recieve_handle);

};


#endif //MEMDB_CONNECTION_H
