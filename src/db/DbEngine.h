//
// Created by xwy on 2018/6/8.
//

#ifndef MEMDB_DBENGINE_H
#define MEMDB_DBENGINE_H

#include <string>
#include <vector>
#include <map>

class DbEngine {
public:
    static const int DB_OK = 0;
    static const int DB_ERROR = -1;
    static const int DB_NOT_FOUND = 1;
    static const int DB_OUT_OF_INDEX = 2;

    //expire time map
    std::multimap<int64_t, std::string> expire_key_map;

    virtual int initialize() = 0;

    int add_expire_key(int64_t time, const std::string& key);

    int delete_expire_keys();

    int64_t get_min_expire_time();

    DbEngine();

    virtual ~DbEngine();

    /**key*/
    virtual int del(const std::string& key, std::string& response) = 0;
    virtual int expire(const std::string& key, int64_t time, std::string& response) = 0;

    /**string*/
    virtual int set(const std::string& key, const std::string& val, std::string &response) = 0;
    virtual int get(const std::string& key, std::string& val) = 0;
    virtual int getrange(const std::string& key, std::string& val, int start, int end) = 0;
    virtual int getset(const std::string& key, std::string& val) = 0;
    virtual int mget(const std::vector<std::string>& keys, std::vector<std::string>& vals) = 0;
//    virtual int setex(const std::string& key, const std::string& val, int seconds);
    virtual int setnx(const std::string& key, std::string& val) = 0;
};


#endif //MEMDB_DBENGINE_H
