//
// Created by xwy on 2018/7/12.
//

#ifndef MEMDB_DATATYPE_H
#define MEMDB_DATATYPE_H


class DataType {
public:
    static const char EXPIRE = 'e';
    static const char STRING = 's';

    static const char MIN_TYPE = EXPIRE;
    static const char MAX_TYPE = STRING;
};


#endif //MEMDB_DATATYPE_H
