//
// Created by python on 18-5-25.
//

#ifndef MEMDB_PROC_H
#define MEMDB_PROC_H

#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include "db/DbEngine.h"
#include "Connection.h"

#define DEF_PROC(f) int proc_##f(DbEngine*, const Request&, Response&)

using proc_t = std::function<int(DbEngine*, const Request &, Response &)>;

class Command {
    static const int FLAG_READ      = (1 << 0);
    static const int FLAG_WRITE     = (1 << 1);
    static const int FLAG_BACKEND   = (1 << 2);
    static const int FLAG_THREAD    = (1 << 3);

    //command name
    std::string name;
    //command flags
    int flags;
    //command
    proc_t proc;
    //minimum argument number
    int min_argc;
    u_int64_t calls;

public:
    Command(): name(), flags(0), proc(nullptr), calls(0) {}

    int initialize(const std::string& name, proc_t proc_fun, int min_argc) {
        this->name = name;
        this->proc = proc_fun;
        this->min_argc = min_argc;
    }

public:
    int operator()(DbEngine* db, const Request & request, Response & response);
};

class Proc {
    std::unordered_map<std::string, Command> proc_map;

public:
    static const int PROCESS_OK = 0;
    static const int PROCESS_ERROR = -1;
    static const int PROCESS_ERROR_MSG = 1;

    int process(Connection* conn, DbEngine* db);

    int set_process(const std::string& command, proc_t proc_fun, int min_arguments);
};

/***************key-value******************/
DEF_PROC(get);
DEF_PROC(set);
DEF_PROC(getrange);

#endif //MEMDB_PROC_H
