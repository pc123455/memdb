//
// Created by xwy on 2018/7/18.
//

#ifndef MEMDB_T_STRING_H
#define MEMDB_T_STRING_H

#include <string>
#include "DataType.h"

std::string encode_string_key(const std::string& key) {
    std::string str_key;
    //2 = length(type) + length('\0')
    str_key.reserve(key.size() + 2);
    str_key.append(1, DataType::STRING);
    str_key.append(key.c_str(), key.size());
    return str_key;
}

#endif //MEMDB_T_STRING_H
