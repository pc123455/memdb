//
// Created by xwy on 2018/5/24.
//

#include "Connection.h"
#include "Logger.h"
#include <unistd.h>
#include <errno.h>
#include <algorithm>

Connection::Connection(): client(nullptr), fd(0), closed(false), buffer(nullptr), buffer_size(0) {

}

int Connection::initialize(fd_t fd, const sockaddr_in *client_addr, uint32_t flags) {
    this->fd = fd;
    if (client == nullptr) {
        //create a client object
        client = new Client("");
    }
    if (buffer == nullptr) {
        //create buffer
        buffer = new Byte[INITIAL_BUFFER_SIZE];
        buffer_size = INITIAL_BUFFER_SIZE;
    }
    if ((flags & FLAG_LISTEN) == 0) {
         //set the client address
         set_clinet_addr((const struct sockaddr*)(client_addr));
    }
    //set flags
    this->flags = flags;
    //set close flag to false
    closed = false;
    //set buffers initial size
    read_buff.reserve(INITIAL_BUFFER_SIZE);
    write_buff.reserve(INITIAL_BUFFER_SIZE);
    //set read/write current pointer
    read_cur_pos = read_buff.begin();
    write_cur_pos = write_buff.begin();


}

int Connection::release() {
    if (client != nullptr) {
        delete client;
        client = nullptr;
    }
    closed = true;
}

int Connection::receive() {
    for(;;) {
        ssize_t n = read(fd, buffer, buffer_size);
        if (n < 0){
            if (errno == EAGAIN) {
                //todo 当前缓冲区已无数据
                //wait a new read event
                return STAGE_AGAIN;
            } else {
                Logger::error(strerror(errno));
                return STAGE_ERROR;
            }
        } else if (n > 0) {
            //insert data into read buffer
            read_buff.insert(read_buff.end(), buffer, buffer + n);
            if (n < buffer_size) {
                return STAGE_OK;
            }
            continue;
        } else {
            // n equals to 0 means file is end
            return STAGE_OK;
        }
    }
}

int Connection::send() {
    write_buff.push_back('+');
    write_buff.push_back('O');
    write_buff.push_back('K');
    write_buff.push_back('\r');
    write_buff.push_back('\n');

    for (;;) {
        ssize_t n = std::min<ssize_t>(write_buff.end() - write_cur_pos, buffer_size);
        if (n < 0) {
            Logger::error("write buffer pointer error");
            return STAGE_ERROR;
        } else if (n == 0) {
            return STAGE_OK;
        }
        std::copy_n(write_cur_pos, n, buffer);
        ssize_t res = write(fd, buffer, n);
        if (res < 0) {
            if (errno == EAGAIN) {
                return STAGE_AGAIN;
            } else {
                Logger::error(strerror(errno));
                return STAGE_ERROR;
            }
        } else {
            //copy data to send buffer
            write_cur_pos += res;
            if (write_cur_pos == write_buff.end()) {
                return STAGE_OK;
            }
            continue;
        }
    }
}

int Connection::get_fd() {
    return fd;
}

void Connection::clear_read_buffer() {
    read_buff.clear();
    read_cur_pos = read_buff.begin();
}

void Connection::clear_write_buffer() {
    write_buff.clear();
    write_cur_pos = write_buff.begin();
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
