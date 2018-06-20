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

#endif //MEMDB_ERROR_H
