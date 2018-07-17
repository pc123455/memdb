//
// Created by xwy on 2018/7/12.
//

#include "LevelDbEngine.h"
#include "DataType.h"
#include "constant.h"



inline std::string encode_expire_key(const std::string key) {
    std::string exp_key;
    //2 = length(type) + length('\0')
    exp_key.reserve(key.size() + 2);
    exp_key.append(1, DataType::EXPIRE);
    exp_key.append(key);
    return exp_key;
}

int LevelDbEngine::del(const std::string &key, std::string &response) {

}

int LevelDbEngine::expire(const std::string& key, int64_t time, std::string& response) {
    std::string new_key = encode_expire_key(key);
    std::string val;
    val.reserve(sizeof(int64_t) + 1);
    val.append((char*)&time, sizeof(int64_t));

    leveldb::Status s = db->Put(leveldb::WriteOptions(), new_key, val);
    if (!s.ok()) {
        response = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }

    response = NUM_1_RESPONSE;
    return DB_OK;
}