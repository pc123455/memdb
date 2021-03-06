//
// Created by python on 18-5-25.
//

#ifndef MEMDB_TYPES_H
#define MEMDB_TYPES_H

#include <sys/types.h>

using Byte = u_char;
using Socket = int;
using fd_t = int;
using Request = std::vector<std::string>;
using Response = std::vector<std::string>;

#endif //MEMDB_TYPES_H
