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

    const static int PARSE_OK = 0;
    const static int PARSE_INVALID_REQUEST = -1;

    //decode redis command from raw data
    static int decode(const std::vector<Byte>& raw, std::vector<std::string>& production);
    //encode data to net data
    static int encode(const std::vector<std::string>& data, std::vector<Byte>& encoded);

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
        auto it = begin;
        while (it != end && *it != '\r') {
            it++;
        }
        std::string temp_str(begin, it);
        int count = std::stoi(temp_str);
        begin = it;
        return count;
    }

    /**
     * parse the bulk string
     * @param begin
     * @param end
     * @param len
     * @return
     */
    static inline int parse_bulk_string(data_const_iterator_t& begin, const data_const_iterator_t& end, int len, std::string& bulk_str) {
        auto it = begin;
        int i = 0;
        for (i = 0; i < len && it != end; i++) {
            it++;
        }
        if (i < len || !validate_end_line(it, end)) {
            return -1;
        }
        bulk_str = std::string(it - 2, end);
        it += 2;
        begin = it;
        return 0;
    }

    /**
     * validate the end of line
     * @param begin
     * @param end
     * @return
     */
    static inline bool validate_end_line(data_const_iterator_t& begin, const data_const_iterator_t& end) {
        if (end - begin <= 2 || *begin != '\r' || *(begin + 1) == '\n') {
            return false;
        }
        begin += 2;
        return true;
    }

//    static inline string
};


#endif //MEMDB_PROTOCOLPARSER_H
