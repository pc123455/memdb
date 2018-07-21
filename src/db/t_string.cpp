//
// Created by xwy on 2018/6/21.
//

#include "LevelDbEngine.h"
#include "constant.h"
#include "DataType.h"
#include "t_string.h"

int encode(std::string& val) {
    std::string resp;
    resp = "$" + std::to_string(val.length()) + "\r\n" + val + "\r\n";
    val = resp;
    return LevelDbEngine::DB_OK;
}

int LevelDbEngine::get(const std::string &key, std::string &val) {
    std::string new_key = encode_string_key(key);

    leveldb::Status s = db->Get(leveldb::ReadOptions(), new_key, &val);
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
    std::string new_key = encode_string_key(key);

    leveldb::Status s = db->Put(leveldb::WriteOptions(), new_key, val);
    if (!s.ok()) {
        response = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }
    response = OK_RESPONSE;
    return DB_OK;
}

int LevelDbEngine::getrange(const std::string &key, std::string &val, int start, int end) {
    std::string str;
    std::string new_key = encode_string_key(key);
    leveldb::Status s = db->Get(leveldb::ReadOptions(), new_key, &str);
    if (s.IsNotFound()) {
        val = NIL_RESPONSE;
        return DB_OK;
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

int LevelDbEngine::getset(const std::string &key, std::string &val) {
    std::string str;
    std::string new_key = encode_string_key(key);
    bool is_not_found = false;
    leveldb::Status s = db->Get(leveldb::ReadOptions(), new_key, &str);
    if (s.IsNotFound()) {
        str = NIL_RESPONSE;
        return DB_OK;
    }
    if (!s.ok()) {
        val = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }

    s = db->Put(leveldb::WriteOptions(), new_key , val);
    if (!s.ok()) {
        val = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }

    encode(val);
    return DB_OK;
}

int LevelDbEngine::mget(const std::vector<std::string> &keys, std::vector<std::string> &vals) {
    std::string str;
    for (auto it_key = keys.begin() + 1; it_key != keys.end(); it_key++) {
        std::string new_key = encode_string_key(*it_key);
        leveldb::Status s = db->Get(leveldb::ReadOptions(), new_key, &str);
        if (s.IsNotFound()) {
            vals.push_back(NIL_RESPONSE);
        }
        if (!s.ok() && !s.IsNotFound()) {
            vals.clear();
            vals.push_back(DB_ERROR_RESPONSE);
            return DB_ERROR;
        }
        encode(str);
        vals.push_back(str);
    }

    return DB_OK;
}

int LevelDbEngine::setnx(const std::string &key, std::string &val) {
    std::string str;
    std::string new_key = encode_string_key(key);
    leveldb::Status s = db->Get(leveldb::ReadOptions(), new_key, &str);
    if (!s.ok() && !s.IsNotFound()) {
        val = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }

    if (s.IsNotFound()) {
        s = db->Put(leveldb::WriteOptions(), key, val);
    }

    if (!s.ok()) {
        val = DB_ERROR_RESPONSE;
        return DB_ERROR;
    }

    val = OK_RESPONSE;
    return DB_OK;
}