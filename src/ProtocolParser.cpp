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
    auto it = raw.begin();
    if (*it != '*') {
        Logger::error("data is invalid");
        return INVALID_REQUEST;
    }
    it++;

    //parse the args count
    auto count_begin = it;
    while (it != raw.end() && *it >= '0' && *it <= '9') {
        it++;
    }
    atoi()

            //simple string
            if(data_len < 3 || !is_end_valid(raw)) {
                //length of "+\r\n" is 3
                Logger::error("data is invalid");
                return -1;
            }

            std::string str(raw.begin() + 1, raw.end() - 2);
            prosduction.push_back(str);
            return 0;

}
