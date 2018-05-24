//
// Created by xwy on 2018/5/24.
//

#include "Connection.h"
#include <unistd.h>

Connection::~Connection() {
    ::close(fd);
}

const std::function<size_t(std::vector<Byte>, size_t)> &Connection::get_send_handle() const {
    return send_handle;
}

void Connection::set_send_handle(const std::function<size_t(std::vector<Byte>, size_t)> &send_handle) {
    Connection::send_handle = send_handle;
}

const std::function<size_t(std::vector<Byte>, size_t)> &Connection::get_recieve_handle() const {
    return recieve_handle;
}

void Connection::set_recieve_handle(const std::function<size_t(std::vector<Byte>, size_t)> &recieve_handle) {
    Connection::recieve_handle = recieve_handle;
}
