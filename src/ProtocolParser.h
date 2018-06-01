//
// Created by xwy on 2018/5/31.
//

#ifndef MEMDB_PROTOCOLPARSER_H
#define MEMDB_PROTOCOLPARSER_H


#include <vector>
#include <string>
#include "utils/types.h"

class ProtocolParser {
public:
    const static int INVALID_REQUEST = -1;

    //decode redis command from raw data
    static int decode(const std::vector<Byte>& raw, std::vector<std::string>& production);
    //encode data to net data
    static int encode(std::vector<std::string>& data, std::vector<Byte>& encoded);

    static inline bool is_end_valid(const std::vector<Byte>& data) {
        size_t len = data.size();
        return data[len - 2] == '\r' && data[len - 1] == '\n';
    }
};


#endif //MEMDB_PROTOCOLPARSER_H
