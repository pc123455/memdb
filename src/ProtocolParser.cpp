//
// Created by xwy on 2018/5/31.
//

#include "ProtocolParser.h"
#include "Logger.h"

int ProtocolParser::decode(const std::vector<Byte>& raw, Request & production) {
    production.clear();
    //validate the raw data length
    size_t data_len = raw.size();
    if (data_len < 4) {
        //the minimum valid request *0\r\n
        Logger::error("data is null");
        return PARSE_INVALID_REQUEST;
    }

    //validate the raw data format
    auto it = raw.begin();
    if (*it != '*') {
        Logger::error("data is invalid");
        return PARSE_INVALID_REQUEST;
    }
    it++;

    //parse the args count
    int count = parse_num(it, raw.end());
    if (count < 0) {
        Logger::error("data is invalid, args num is 0!");
        return PARSE_INVALID_REQUEST;
    }
    //validate end of line
    if (!validate_end_line(it, raw.end())) {
        Logger::error("data is invalid, end line isn't \\r\\n!");
        return PARSE_INVALID_REQUEST;
    }

    for (int i = 0; i < count; i++) {
        //validate iterator is valid
        if (it == raw.end()) {
            Logger::error("data is invalid, params count error!");
            return PARSE_INVALID_REQUEST;
        }
        if (*it != '$') {
            Logger::error("data is invalid, bulk strings begin without $!");
            return PARSE_INVALID_REQUEST;
        }
        it++;

        //parse thr bulk string length
        int len = parse_num(it, raw.end());
        if (len <= 0) {
            Logger::error("data is invalid, bulk string length is 0 or smaller than 0");
            return PARSE_INVALID_REQUEST;
        }
        if (!validate_end_line(it, raw.end())) {
            Logger::error("data is invalid, end line isn't \\r\\n!");
            return PARSE_INVALID_REQUEST;
        }

        //parse the bulk string
        std::string str;
        if (parse_bulk_string(it, raw.end(), len, str) == -1) {
            Logger::error("data is invalid, bulk string invalid");
            return PARSE_INVALID_REQUEST;
        }
        production.push_back(str);
    }

    return PARSE_OK;
}

int ProtocolParser::encode(const Response &data, std::vector<Byte> &encoded) {
    encoded.reserve(1024);
//    std::string len_str = "$" + std::to_string(data.size()) + "\r\n";
//    encoded.insert(encoded.end(), len_str.begin(), len_str.end());
    for (auto it = data.begin(); it != data.end(); it++) {
        encoded.insert(encoded.end(), it->begin(), it->end());
    }
    return ENCODE_OK;
}

int ProtocolParser::copy_data(const Response &data, std::vector<Byte> &encode) {
    encode.reserve(1024);
    encode.insert(encode.end(), data[0].begin(), data[0].end());
    return ENCODE_OK;
}