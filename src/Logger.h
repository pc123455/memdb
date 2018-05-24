//
// Created by python on 18-5-24.
//

#ifndef MEMDB_LOGGER_H
#define MEMDB_LOGGER_H

#include <iostream>


class Logger {
public:
    static void init();

    static std::ostream& info(const char* msg);

    static std::ostream& error(const char* error);
};


#endif //MEMDB_LOGGER_H
