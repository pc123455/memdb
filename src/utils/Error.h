//
// Created by xwy on 2018/6/11.
//

#ifndef MEMDB_ERROR_H
#define MEMDB_ERROR_H

#include <string>

class Error {
public:
    static const std::string INVALID_COMMAND;
};

const std::string Error::INVALID_COMMAND = "-invalid commond\r\n";
#endif //MEMDB_ERROR_H
