//
// Created by xwy on 2018/5/31.
//

#ifndef MEMDB_PROTOCOLPARSER_H
#define MEMDB_PROTOCOLPARSER_H


#include <vector>
#include "utils/types.h"

class ProtocolParser {
    //decode redis command from raw data
    static int decode(std::vector<Byte>& raw, std::vector<Byte>& production);
    //encode data to net data
    static int encode(std::vector<std::string> data, std::vector<Byte>& encoded);
};


#endif //MEMDB_PROTOCOLPARSER_H
