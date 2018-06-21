//
// Created by xwy on 2018/6/11.
//

#include "LevelDbEngine.h"
#include "../Logger.h"
#include "constant.h"

LevelDbEngine::LevelDbEngine(): db(nullptr) {

}

int LevelDbEngine::initialize() {
    data_file = "/home/xwy/testdb";
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, data_file, &db);
    if (!status.ok()) {
        Logger::error("database initialize error!");
        return DB_ERROR;
    }
    return DB_OK;
}

LevelDbEngine::~LevelDbEngine() {
    delete db;
}
