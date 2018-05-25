//
// Created by python on 18-5-25.
//

#ifndef MEMDB_PROC_H
#define MEMDB_PROC_H

#include <functional>
#include <string>

using proc_t = std::function<int (std::vector<std::string>, std::vector<std::string>)>;

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
    u_int64_t calls;

    Command(): name(), flags(0), proc(nullptr), calls(0) {}
};

class Proc {

};


#endif //MEMDB_PROC_H
