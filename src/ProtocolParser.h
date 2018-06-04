//
// Created by xwy on 2018/5/31.
//

#ifndef MEMDB_PROTOCOLPARSER_H
#define MEMDB_PROTOCOLPARSER_H


#include <vector>
#include <string>
#include <exception>
#include "utils/types.h"

class ProtocolParser {
public:
    using data_iterator_t = std::vector<Byte>::iterator;
    using data_const_iterator_t = std::vector<Byte>::const_iterator;

    const static int INVALID_REQUEST = -1;

    //decode redis command from raw data
    static int decode(const std::vector<Byte>& raw, std::vector<std::string>& production);
    //encode data to net data
    static int encode(std::vector<std::string>& data, std::vector<Byte>& encoded);

    static inline bool is_end_valid(const std::vector<Byte>& data) {
        size_t len = data.size();
        return data[len - 2] == '\r' && data[len - 1] == '\n';
    }

    /**
     * parse the first integer number between and end
     * @param begin the iterator of first number
     * @param end the last iterator
     * @return parsed number, and the begin iterator will be set to the end of number
     */
    static inline int parse_num(data_const_iterator_t& begin, const data_const_iterator_t& end) {
        data_const_iterator_t it = begin;
        while (it != end && *it != '\r') {
            it++;
        }
        std::string temp_str(begin, it);
        int count = std::stoi(temp_str);
        begin = it;
        return count;
    }

    /**
     * validate the end of line
     * @param begin
     * @param end
     * @return
     */
    static inline bool validate_end_line(data_const_iterator_t& begin, const data_const_iterator_t& end) {
        if (end - begin < 2 || *begin != '\r' || *(begin + 1) == '\n') {
            return false;
        }
        begin += 2;
        return true;
    }

//    static inline string
};


#endif //MEMDB_PROTOCOLPARSER_H
