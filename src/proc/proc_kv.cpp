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
    return Proc::PROCESS_OK;
}