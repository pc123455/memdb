//
// Created by python on 18-5-25.
//

#include "Proc.h"
#include "ProtocolParser.h"
#include "utils/Error.h"
#include "db/DbEngine.h"

int Command::operator()(DbEngine* db, const Request & request, Response & response) {
    if (request.size() < min_argc) {
        response.push_back(Error::WRONG_ARGUMENT_NUM);
        return Proc::PROCESS_OK;
    }
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

    //find the process function
    auto proc_it = proc_map.find(request[0]);
    if (proc_it == proc_map.end()) {
        response.push_back(Error::INVALID_COMMAND);
        ProtocolParser::copy_data(response, conn->get_write_buffer());
        return PROCESS_OK;
    }

    //call process function to get response
    Command command = proc_it->second;
    res = command(db, request, response);
    if(res == DbEngine::DB_ERROR) {
        return PROCESS_ERROR;
    }

    //encode response and move to write buffer
    switch (res) {
        case DbEngine::DB_OK:
            ProtocolParser::encode(response, conn->get_write_buffer());
            break;
        case PROCESS_ERROR_MSG:
            ProtocolParser::copy_data(response, conn->get_write_buffer());
            break;
        case DbEngine::DB_NOT_FOUND:
            ProtocolParser::copy_data(response, conn->get_write_buffer());
            break;
        default:
            return PROCESS_ERROR;
    }

    return PROCESS_OK;
}

int Proc::set_process(const std::string& command, proc_t proc_fun, int min_arguments) {
    auto proc_it = proc_map.find(command);
    //command exist
    if (proc_it != proc_map.end()) {
        return PROCESS_ERROR;
    }

    std::pair<std::string, Command> p(command, Command());
//    p.first = command;
    p.second.initialize(command, proc_fun, min_arguments);
    proc_map.insert(p);

    return PROCESS_OK;
}