//
// Created by xwy on 2018/7/2.
//
#include "../Proc.h"
#include "../utils/types.h"
#include "../utils/response.h"
#include "../config/Config.h"

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
    Config* config = Config::get_instance();
    if (!config->is_require_pass) {
        response.push_back(PASSWORD_UNREQUIRED);
    } else if (client->authenticate(pwd)) {
        response.push_back(OK_RESPONSE);
    } else {
        response.push_back(PASSWORD_WRONG);
    }
    return Proc::PROCESS_OK;
}
