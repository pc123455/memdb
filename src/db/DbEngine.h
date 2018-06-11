//
// Created by xwy on 2018/6/8.
//

#ifndef MEMDB_DBENGINE_H
#define MEMDB_DBENGINE_H

#include <string>
#include <vector>

class DbEngine {
public:
    static const int DB_OK;
    static const int DB_ERROR;

    virtual int initialize();

    virtual int set(const std::string& key, const std::string& val);

    virtual int get(const std::string& key, std::string& val);

    virtual int get_range(const std::string& key, std::string& val, int start, int end);

    virtual int get_set(const std::string& key, std::string& val);

    virtual int mget(const std::vector<std::string>& keys, std::vector<std::string>& vals);

    virtual int setex(const std::string& key, const std::string& val, int seconds);

    virtual int setnx(const std::string& key, const std::string& val);
};


#endif //MEMDB_DBENGINE_H
