//
// Created by xwy on 2018/5/31.
//

#ifndef MEMDB_BUFFER_H
#define MEMDB_BUFFER_H


#include <cstddef>

class Buffer {
public:
    //buffer array
    char* buff;
    //size
    size_t size;
    //used size
    size_t used;
};


#endif //MEMDB_BUFFER_H
