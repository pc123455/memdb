//
// Created by xwy on 2018/5/31.
//

#include "ProtocolParser.h"
#include "Logger.h"

int ProtocolParser::decode(const std::vector<Byte>& raw, std::vector<std::string>& production) {
    production.clear();
    //validate the raw data length
    size_t data_len = raw.size();
    if (data_len < 4) {
        //the minimum valid request *0\r\n
        Logger::error("data is null");
        return INVALID_REQUEST;
    }

    //validate the raw data format
    data_const_iterator_t it = raw.begin();
    if (*it != '*') {
        Logger::error("data is invalid");
        return INVALID_REQUEST;
    }
    it++;

    //parse the args count
    int count = parse_num(it, raw.end());
    if (count < 0) {
        Logger::error("data is invalid, args num is 0!");
        return INVALID_REQUEST;
    }
    //validate end of line
    if (!validate_end_line(it, raw.end())) {
        Logger::error("data is invalid, end line isn't \\r\\n!");
        return INVALID_REQUEST;
    }

    for (int i = 0; i < count; i++) {
        //validate iterator is valid
        if (it == raw.end()) {
            Logger::error("data is invalid, params count error!");
            return INVALID_REQUEST;
        }
        if (*it != '$') {
            Logger::error("data is invalid, bulk strings begin without $!");
            return INVALID_REQUEST;
        }
        it++;

        //parse thr bulk string length
        int len = parse_num(it, raw.end());
        if (!validate_end_line(it, raw.end())) {
            Logger::error("data is invalid, end line isn't \\r\\n!");
            return INVALID_REQUEST;
        }


    }

            //simple string
            if(data_len < 3 || !is_end_valid(raw)) {
                //length of "+\r\n" is 3
                Logger::error("data is invalid");
                return -1;
            }

            std::string str(raw.begin() + 1, raw.end() - 2);
            production.push_back(str);
            return 0;

}
