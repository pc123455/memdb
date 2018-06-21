//
// Created by xwy on 2018/6/11.
//

#ifndef MEMDB_LEVELDBENGINE_H
#define MEMDB_LEVELDBENGINE_H

#include "DbEngine.h"
#include <leveldb/db.h>

class LevelDbEngine: public DbEngine {
    leveldb::DB* db;
    leveldb::Options options;
    std::string data_file;

public:
    int initialize() override;

    int get(const std::string& key, std::string& val) override;
    int set(const std::string &key, const std::string &val, std::string &response) override;
    int getrange(const std::string& key, std::string& val, int start, int end) override;

    LevelDbEngine();

    ~LevelDbEngine();
};


#endif //MEMDB_LEVELDBENGINE_H