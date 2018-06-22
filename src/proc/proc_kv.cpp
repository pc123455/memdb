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
    return result == DbEngine::DB_OK ? Proc::PROCESS_OK : Proc::PROCESS_ERROR_MSG;
}

int proc_set(DbEngine* db, const Request & request, Response & response) {
    std::string resp;
    int result = db->set(request[0], request[1], resp);
    response.push_back(resp);
    return result == DbEngine::DB_OK ? Proc::PROCESS_OK : Proc::PROCESS_ERROR_MSG;
}

int proc_getrange(DbEngine* db, const Request & request, Response & response) {
    std::string val;
    int start = std::stoi(request[1]);
    int end = std::stoi(request[2]);
    int result = db->getrange(request[0], val, start, end);
    response.push_back(val);
    return result == DbEngine::DB_OK ? Proc::PROCESS_OK : Proc::PROCESS_ERROR_MSG;
}