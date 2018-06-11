//
// Created by python on 18-5-25.
//

#include "Proc.h"
#include "ProtocolParser.h"
#include "utils/Error.h"

int Command::operator()(DbEngine* db, std::vector<std::string>& request, std::vector<std::string>& response) {
    return proc(db, request, response);
}

int Proc::process(Connection *conn, DbEngine* db) {
    //decode request
    std::vector<std::string> request;
    std::vector<std::string> response;
    int res = ProtocolParser::decode(conn->get_read_buffer(), request);
    if (res == ProtocolParser::PARSE_INVALID_REQUEST) {
        return PROCESS_ERROR;
    }

    //process request
    if (request.size() == 0) {
        return PROCESS_ERROR;
    }

    auto proc_it = proc_map.find(request[0]);

    if (proc_it == proc_map.end()) {
        response.push_back(Error::INVALID_COMMOND);
        return PROCESS_OK;
    }

    Command command = proc_it->second;
    res = command(db, request, response);
    if(res == PROCESS_ERROR) {
        return PROCESS_ERROR;
    }

    conn->set_write_buffer()
}