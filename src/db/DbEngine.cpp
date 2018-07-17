//
// Created by xwy on 2018/6/8.
//

#include "DbEngine.h"
#include "../utils/time.h"
#include "../Logger.h"

//const int DbEngine::DB_OK = 0;
//const int DbEngine::DB_ERROR = -1;
//const int DbEngine::DB_NOT_FOUND = 1;
//const int DbEngine::DB_OUT_OF_INDEX = 2;

DbEngine::~DbEngine() {}

DbEngine::DbEngine() {}

int DbEngine::add_expire_key(int64_t time, const std::string &key) {
    expire_key_map.insert(std::make_pair(time, key));
    return 0;
}

int DbEngine::delete_expire_keys() {
    //delete expired keys
    int64_t time = time_ms();
    for (auto it = expire_key_map.begin(); it != expire_key_map.end(); it++) {
        if (it->first <= time) {
            //delete expired keys
            std::string response;
            int res = del(it->second, response);
            if (res != DB_OK) {
                Logger::error("del expire key error!");
                return DB_ERROR;
            }
        } else {
            break;
        }
    }
    return DB_OK;
}

int64_t DbEngine::get_min_expire_time() {
    auto it = expire_key_map.begin();
    if (it != expire_key_map.end()) {
        return it->first;
    }
    return -1;
}