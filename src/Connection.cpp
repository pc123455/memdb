//
// Created by xwy on 2018/5/24.
//

#include "Connection.h"
#include <unistd.h>

Connection::Connection(): client(nullptr), fd(0), closed(false) {

}

int Connection::initialize(fd_t fd, const sockaddr_in *client_addr) {
    if (client == nullptr) {
        //create a client object
        client = new Client("");
    }
    //set the client address
    set_clinet_addr((const struct sockaddr*)(client_addr));
    //set close flag to false
    closed = false;
    //set buffers
    read_buff = std::vector<Byte>(INITIAL_BUFFER_SIZE);
    read_buff = std::vector<Byte>(INITIAL_BUFFER_SIZE);
}

int Connection::release() {
    if (client != nullptr) {
        delete client;
        client = nullptr;
    }
    closed = true;
}

int Connection::receive() {
    size_t s = read(fd, )
}

Connection::~Connection() {
    if (client != nullptr) {
        delete client;
        client = nullptr;
    }
    ::close(fd);
}

#define REG_PROC(c, f)     net->proc_map.set_proc(#c, f, proc_##c)

//const std::function<size_t(std::vector<Byte>&, size_t)> &Connection::get_send_handle() const {
//    return send_handle;
//}
//
//void Connection::set_send_handle(const std::function<size_t(std::vector<Byte>&, size_t)> &send_handle) {
//    this->send_handle = send_handle;
//}
//
//const std::function<size_t(std::vector<Byte>&, size_t)> &Connection::get_recieve_handle() const {
//    return receive_handle;
//}
//
//void Connection::set_recieve_handle(const std::function<size_t(std::vector<Byte>&, size_t)> &recieve_handle) {
//    this->receive_handle = recieve_handle;
//}
