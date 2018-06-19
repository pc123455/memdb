//
// Created by xwy on 2018/6/11.
//

#ifndef MEMDB_ERROR_H
#define MEMDB_ERROR_H

#include <string>

class Error {
public:
    static const std::string INVALID_COMMAND;
    static const std::string WRONG_ARGUMENT_NUM;
};

const std::string Error::INVALID_COMMAND = "-invalid commond\r\n";
const std::string Error::WRONG_ARGUMENT_NUM = "-wrong number of arguments\r\n";
#endif //MEMDB_ERROR_H
