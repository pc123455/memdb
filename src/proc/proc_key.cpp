//
// Created by xwy on 2018/7/12.
//

#include "../db/DbEngine.h"
#include "../utils/types.h"
#include "../Proc.h"
#include "../utils/time.h"
#include <string>


int proc_del(Client* client, DbEngine* db, const Request & request, Response & response) {
    return Proc::PROCESS_OK;
}

int proc_expire(Client* client, DbEngine* db, const Request & request, Response & response) {
    std::string resp;
    int64_t time = std::stoll(request[2]) * 1000 + time_ms();
    int result = db->expire(request[1], time, resp);
    response.push_back(resp);

    //todo 更新超时红黑树

    return result == DbEngine::DB_OK ? Proc::PROCESS_OK : Proc::PROCESS_ERROR_MSG;
}