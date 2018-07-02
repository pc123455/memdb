//
// Created by xwy on 2018/7/2.
//
#include "../Proc.h"
#include "../utils/types.h"
#include "../utils/response.h"

int string_encode(std::string& val) {
    std::string resp;
    resp = "$" + std::to_string(val.length()) + "\r\n" + val + "\r\n";
    val = resp;
    return Proc::PROCESS_OK;
}

int proc_ping(Client* client, DbEngine* db, const Request & request, Response & response) {
    if (request.size() > 1) {
        response.push_back(request[1]);
        string_encode(response[0]);
    } else {
        response.push_back(PONG_RESPONSE);
    }
    return Proc::PROCESS_OK;
}

int proc_auth(Client* client, DbEngine* db, const Request & request, Response & response) {
    std::string pwd = request[1];
    if (client->authenticate(pwd)) {
        response.push_back(OK_RESPONSE);
    } else {
        response.push_back(PASSWORD_WORNG);
    }
    return Proc::PROCESS_OK;
}
