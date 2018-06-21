//
// Created by xwy on 2018/6/21.
//

#include "LevelDbEngine.h"
#include "constant.h"

int encode(std::string& val) {
    std::string resp;
    resp = "$" + std::to_string(val.length()) + "\r\n" + val + "\r\n";
    val = resp;
    return LevelDbEngine::DB_OK;
}

int LevelDbEngine::get(const std::string &key, std::string &val) {
    leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &val);
    if (!s.ok()) {
        val = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }
    if (s.IsNotFound()) {
        val = NIL_RESPONSE;
    } else {
        encode(val);
    }
    return DB_OK;
}

int LevelDbEngine::set(const std::string &key, const std::string &val, std::string &response) {
    leveldb::Status s = db->Put(leveldb::WriteOptions(), key, val);
    if (!s.ok()) {
        response = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }
    response = OK_RESPONSE;
    return DB_OK;
}

int LevelDbEngine::getrange(const std::string &key, std::string &val, int start, int end) {
    std::string str;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &str);
    if (s.IsNotFound()) {
        val = NIL_RESPONSE;
    }
    if (!s.ok()) {
        val = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }

    ssize_t len = str.length();
    //ensure start and end is larger than 0
    if (start < 0) {
        start = std::max<int>(len + start, 0);
    }
    if (end < 0) {
        end = std::max<int>(len + end, 0);
    }

    //ensure start and end is smaller than len
    start = std::min<int>(start, len - 1);
    end = std::min<int>(end, len - 1);
    //ensure end is larger than start
    end = std::max(start, end);

    val = std::string(str.begin() + start, str.begin() + end);
    encode(val);
    return DB_OK;
}