//
// Created by xwy on 2018/6/19.
//

#include "../db/DbEngine.h"
#include "../utils/types.h"
#include "../Proc.h"

int proc_get(DbEngine* db, const Request & request, Response & response) {
    std::string val;
    int result = db->get(request[0], val);
    response.push_back(val);
    return result;
}

int proc_set(DbEngine* db, const Request & request, Response & response) {
    std::string resp;
    int result = db->set(request[0], request[1], resp);
    if (result == DbEngine::DB_OK) {
        response.push_back(resp);
    }
}

int proc_getrange(DbEngine* db, const Request & request, Response & response) {
    std::string val;
    int start = std::stoi(request[1]);
    int end = std::stoi(request[2]);
    int result = db->getrange(request[0], val, start, end);

}