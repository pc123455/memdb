//
// Created by xwy on 2018/5/24.
//

#include "Connection.h"
#include <unistd.h>

Connection::Connection(): client(nullptr), fd(0), closed(false) {

}

Connection::~Connection() {
    delete client;
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
